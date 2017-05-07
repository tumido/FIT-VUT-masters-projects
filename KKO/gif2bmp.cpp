#include "gif2bmp.h"
using namespace std;

u_int8_t get_byte(FILE * file) {
  u_int8_t c = fgetc(file);
  if (c == EOF) {
    eprintf("Unexpected GIF file end");
    exit(EXIT_FAILURE);
  }
  return c;
}

bool verify_gif_signature(FILE * file) {
  vector<u_int8_t> data;
  for (size_t i = 0; i < GIF_SIGNATURE_LENGTH; i++)
    data.push_back(get_byte(file));

  string signature(data.begin(), data.end());
  return (signature == GIF_SIGNATURE);
}

tGIFInfo read_gif_header(FILE * file) {
  tGIFInfo info;
  info.width  = get_byte(file) | (get_byte(file) << BYTE);
  info.height = get_byte(file) | (get_byte(file) << BYTE);
  info.gct_flags = get_byte(file);
  info.bg_color = get_byte(file);
  info.px_ratio = get_byte(file);
  return info;
}

vector<tRGB> read_color_table(FILE * file, size_t size) {
  vector<tRGB> table;
  for(size_t i = 0; i < size; i++)
    table.emplace_back(get_byte(file), get_byte(file), get_byte(file));

  return table;
}

deque<u_int8_t> get_block_data(FILE * file) {
  deque<u_int8_t> data;
  u_int8_t b_size;
  while ((b_size = get_byte(file)) != TERMINATOR) {
    for (u_int8_t i = 0; i < b_size; i++) {
      data.push_back(get_byte(file));
    }
  }
  return data;
}

void parse_image_block(FILE * file, vector<tRGB> * image_data, vector<tRGB> * gct, tGraphicExt * gext) {
  // Read left and top position, height and width
  u_int16_t left_pos = get_byte(file) | (get_byte(file) << BYTE);
  u_int16_t top_pos = get_byte(file) | (get_byte(file) << BYTE);
  u_int16_t width = get_byte(file) | (get_byte(file) << BYTE);
  u_int16_t height = get_byte(file) | (get_byte(file) << BYTE);
  u_int8_t lct_flags = get_byte(file);

  // Parse local color table
  vector<tRGB> lct;
  if (lct_flags & (1 << COLOR_TABLE_ENABLED_OFFSET)) {
    size_t size = 1 << ((lct_flags & LCT_SIZE_MASK)+1);
    lct = read_color_table(file, size);
  }

  // Read LZW minimal code size
  u_int8_t lzw_min_code_size = get_byte(file);

  // Read data of all sub-blocks
  deque<u_int8_t> data = get_block_data(file);

  // Select color table
  vector<tRGB> * color_table = lct.size() ? &lct : gct;

  // Decompress LZW
  u_int16_t clear_code = 1 << lzw_min_code_size;
  u_int16_t lzw_real_code_size = lzw_min_code_size + 1;
  u_int16_t end_input = clear_code + 1;
  u_int16_t last_code = clear_code - 1;

  // Init Dictionary
  unordered_map<u_int16_t, vector<tRGB>> dict(clear_code);
  for (u_int16_t j = 0; dict.size() < color_table->size(); j++ ){
    dict.emplace(j, vector<tRGB>());
    dict[j].push_back(color_table->at(j));
  }
  vector<tRGB> previous_data;
  u_int16_t code;
  u_int16_t counter = 0;
  u_int8_t mask = 1;
  u_int8_t bit;
  bool first_flag = true;
  u_int16_t table_idx = 0;
  while (counter < data.size()) {
    code = 0;
    for(u_int8_t idx = 0; idx < lzw_real_code_size; idx++) {
      bit = (data[counter] & mask) ? 1 : 0;
      code = code | (bit << idx);
      mask = mask << 1;
      if (!mask) {
        counter++;
        mask =1;
      }
    }
    // cout << to_string(code) << " :" << to_string(lzw_real_code_size) << endl;
    if (first_flag) {
      first_flag = false;
      continue;
    }
    table_idx++;

    if (code == end_input)
      break;

    if (code == clear_code){
      dict.clear();
      lzw_real_code_size = lzw_min_code_size + 1;
      last_code = (1 << lzw_min_code_size) - 1;
      // printf("CLEAR %d\n", clear_code);
      table_idx = 0;
      dict = unordered_map<u_int16_t, vector<tRGB>>(1 << lzw_min_code_size);
      for (u_int16_t i = 0; i < color_table->size(); i++) {
        dict.emplace(i, vector<tRGB>());
        dict[i].push_back(color_table->at(i));
      }
    }

    if (table_idx == last_code) {
      // printf("LAST\n");
      if (lzw_real_code_size < MAX_LZW_SIZE)
        lzw_real_code_size++;
      last_code = 1 << (lzw_real_code_size-1);
      table_idx = 0;
    }

    // FIXME
    if (dict.find(code) == dict.end() && previous_data.size())
      dict[code].push_back(previous_data[0]);

    image_data->insert(image_data->end(), dict[code].begin(), dict[code].end());

    if (previous_data.size()){
      dict[dict.size()] = previous_data;
      dict.emplace(dict.size(), previous_data);
      dict[dict.size()].push_back(dict[code][0]);
    }
    previous_data = dict[code];
  }
}


void parse_text_ext(FILE * file) {
  // Skip it
  get_block_data(file);
}

void parse_comment_ext(FILE * file) {
  // Skip it
  get_block_data(file);
}

tGraphicExt parse_graphic_ext(FILE * file) {
  tGraphicExt data;
  // Skip first byte telling the extension size
  get_byte(file);

  data.packed_field = get_byte(file);
  data.delay_time = get_byte(file) | (get_byte(file) << BYTE);
  data.transparent_color_idx = get_byte(file);

  // TERMINATOR
  get_byte(file);
  return data;
}

void parse_app_ext(FILE * file) {
  // Skip first byte telling the extension size
  get_byte(file);
  // Get app identifier
  vector<u_int8_t> id_data;
  for (size_t i = 0; i < APP_ID_SIZE; i++)
    id_data.push_back(get_byte(file));
  string app_id(id_data.begin(), id_data.end());
  // Print it
  eprintf("Application extension block: %s", app_id.c_str());
  // Skip the rest
  get_block_data(file);
}

vector<tRGB> parse_gif_content(FILE * file, vector<tRGB> * gct) {
  vector<tRGB> image;
  // Read separators until end
  u_int8_t sep;
  while((sep = get_byte(file)) != TRAILER) {
    // Decide if it is IMAGE_BLOCK or extension
    tGraphicExt g_ext = {};
    switch(sep) {
      case IMAGE_BLOCK:
        parse_image_block(file, &image, gct, &g_ext);
        break;
      case EXTENSION_BLOCK: {
        u_int8_t ext_type = get_byte(file);
        switch(ext_type) {
          case GRAPHIC_EXT:
            parse_graphic_ext(file);
            break;
          case COMMENT_EXT:
            parse_comment_ext(file);
            break;
          case TEXT_EXT:
            parse_text_ext(file);
            break;
          case APP_EXT:
            parse_app_ext(file);
            break;
          default:
            eprintf("Unknown extension block '%x'", ext_type);
            return image;
        }
        break;
      }
      default:
        eprintf("Unknown block introducer '%x'", sep);
        return image;
    }
  }
  return image;
}

void dump_bmp(FILE * file, tGIFInfo info, vector<tRGB> data) {
  // Signature
	fwrite(BMP_SIGNATURE, sizeof(uint16_t), 1, file);
  // Size
  u_int32_t align = info.width*sizeof(tRGB);
  align = BMP_ALIGN - (align % BMP_ALIGN);
  u_int32_t bytes = BMP_HEADER_SIZE + BMP_HEADER_OFFSET + info.width*info.height*sizeof(tRGB) + info.height*align;
	fwrite(&bytes, sizeof(u_int32_t), 1, file);
  // Reserved 2x
	bytes = 0;
	fwrite(&bytes, sizeof(u_int32_t), 1, file);
  // Data offset
  bytes = BMP_HEADER_OFFSET;
	fwrite(&bytes, sizeof(u_int32_t), 1, file);
  // Header size
	fwrite(&bytes, sizeof(u_int32_t), 1, file);
  // Dimensions
	fwrite(&(info.width), sizeof(u_int32_t), 1, file);
	fwrite(&(info.height), sizeof(u_int32_t), 1, file);
  // Additional data
  bytes = BMP_N_PLANES;
	fwrite(&bytes, sizeof(u_int16_t), 1, file);
  bytes = BMP_BPP;
	fwrite(&bytes, sizeof(u_int16_t), 1, file);
  bytes = BMP_COMPRESS;
	fwrite(&bytes, sizeof(u_int32_t), 1, file);
  bytes = data.size();
	fwrite(&bytes, sizeof(u_int32_t), 1, file);
  u_int32_t zero[4] = {BMP_RES, BMP_RES, BMP_COLORS, BMP_COLORS};
	fwrite(zero, sizeof(u_int32_t), 4, file);

  for (auto i: data) {
    fwrite(&(i.b), sizeof(u_int8_t), 1, file);
    fwrite(&(i.g), sizeof(u_int8_t), 1, file);
    fwrite(&(i.r), sizeof(u_int8_t), 1, file);
  }
  for (size_t i = 0; i < align; i++) fputc(0, file);
}

int gif2bmp(tGIF2BMP *gif2bmp, FILE *inputFile, FILE *outputFile) {
  // Verify the inputFile is a GIF
  if (!verify_gif_signature(inputFile)) {
    return EXIT_FAILURE;
  }

  // Setup GIF related information storage
  tGIFInfo info = read_gif_header(inputFile);

  // Initialize global color table
  vector<tRGB> global_color_table;
  if (info.gct_flags >> COLOR_TABLE_ENABLED_OFFSET) {
    size_t size = 1 << ((info.gct_flags & GCT_SIZE_MASK)+1);
    global_color_table = read_color_table(inputFile, size);
  }

  // Process the file content
  vector<tRGB> image = parse_gif_content(inputFile, &global_color_table);

  // Match the end of file
  gif2bmp->gifSize = ftell(inputFile);
  fseek(inputFile, 0, SEEK_END);
  if (gif2bmp->gifSize != ftell(inputFile)){
    eprintf("Unexpected file size (analyzed size doesn't match with real size)\n");
    return EXIT_FAILURE;
  }

  dump_bmp(outputFile, info, image);

  return EXIT_SUCCESS;
}