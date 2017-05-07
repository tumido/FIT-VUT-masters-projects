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

void parse_image_block(FILE * file, vector<vector<tRGB>> * image_data, vector<tRGB> * gct, tGraphicExt * gext) {
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
  // interlacing? lct_flags & (1 << LCT_INTERLACE_OFFSET);

  // Read LZW minimal code size
  u_int8_t lzw_min_code_size = get_byte(file);

  // Read data of all sub-blocks
  deque<u_int8_t> data = get_block_data(file);

  // Process data
  // clear_code 1 << lzw_min_code_size
  // end_input (1 << lzw_min_code_size) + 1
  // lzw_code_size lzw_min_code_size + 1
  // for (auto b: data) {
  //
  // }
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

vector<vector<tRGB>> parse_gif_content(FILE * file, vector<tRGB> * gct) {
  vector<vector<tRGB>> image;
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
  vector<vector<tRGB>> image = parse_gif_content(inputFile, &global_color_table);

  // Match the end of file
  gif2bmp->gifSize = ftell(inputFile);
  fseek(inputFile, 0, SEEK_END);
  if (gif2bmp->gifSize != ftell(inputFile)){
    eprintf("Unexpected file size (analyzed size doesn't match with real size)\n");
    return EXIT_FAILURE;
  }
    //
    // // Zápis získaných dat do výstupního souboru
    // writeBMPData(gif2bmp);
    //
    // // Uložení velikosti GIF souboru pro log
    // if(gif2bmp != NULL) gif2bmp->gifSize = gifSize;
    //

  return EXIT_SUCCESS;
}