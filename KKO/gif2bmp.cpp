#include "gif2bmp.h"
using namespace std;

/**
 * Read a Byte from file
 * @param  file input
 * @return      a byte of data
 */
u_int8_t get_byte(FILE * file) {
  u_int8_t c = fgetc(file);
  if (c == EOF) {
    eprintf("Unexpected GIF file end");
    exit(EXIT_FAILURE);
  }
  return c;
}

/**
 * Check if the GIF type and signature is present and matching
 * @param  file input
 * @return      true if the signature is valid
 */
bool verify_gif_signature(FILE * file) {
  vector<u_int8_t> data;
  for (size_t i = 0; i < GIF_SIGNATURE_LENGTH; i++)
    data.push_back(get_byte(file));

  string signature(data.begin(), data.end());
  return (signature == GIF_SIGNATURE);
}

/**
 * Parse GIF header
 * @param  file input
 * @return      a header according to tGIFInfo type
 */
tGIFInfo read_gif_header(FILE * file) {
  tGIFInfo info;
  // Read 2 byte values
  info.width  = get_byte(file) | (get_byte(file) << BYTE);
  info.height = get_byte(file) | (get_byte(file) << BYTE);
  // Read 1 byte values
  info.gct_flags = get_byte(file);
  info.bg_color = get_byte(file);
  info.px_ratio = get_byte(file);
  return info;
}

/**
 * Parse any color table
 * @param file       input
 * @param size       the size specification of the table
 * @return           vector of colors in a table
 */
vector<tRGB> read_color_table(FILE * file, size_t size) {
  vector<tRGB> table;
  // Push colors to vector
  for(size_t i = 0; i < size; i++)
    table.emplace_back(get_byte(file), get_byte(file), get_byte(file));

  return table;
}

/**
 * Parse block data (including sub-blocks) and return them as a vector
 * @param file       input
 * @return           deque of block data
 */
deque<u_int8_t> get_block_data(FILE * file) {
  deque<u_int8_t> data;
  u_int8_t b_size;
  // Read data until a TERMINATOR is found
  while ((b_size = get_byte(file)) != TERMINATOR) {
    for (u_int8_t i = 0; i < b_size; i++) {
      data.push_back(get_byte(file));
    }
  }
  return data;
}

void add_to_dic(vector< std::vector<int> > & dic, const vector<int> & add, size_t size) {
	for (size_t i = 0; i < size; i++) {
		if (dic[i].size() == add.size()) {
			for (size_t j = 0; j < dic[i].size(); ++j) {
				if (dic[i][j] == add[j] && j == dic[i].size() - 1) {
					return;
				} else if (dic[i][j] != add[j])
					break;
			}
		}
	}
	dic.push_back(add);
}
/**
 * Process the image data in a file
 * @param file       input
 * @param image_data vector of output data
 * @param gct        global color map
 * @param gext       graphic extension structure
 */
void parse_image_block(FILE * file, vector<u_int8_t> * image_data, vector<tRGB> * gct, tGraphicExt * gext,
vector<tRGB> ** used_ct) {
  // Read left and top position, height and width
  u_int16_t skipped = get_byte(file) | (get_byte(file) << BYTE);
  skipped = get_byte(file) | (get_byte(file) << BYTE);
  skipped = get_byte(file) | (get_byte(file) << BYTE);
  skipped = get_byte(file) | (get_byte(file) << BYTE);
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
  *used_ct = lct.size() ? &lct : gct;

  // Decompress LZW
  u_int16_t clear_code = 1 << lzw_min_code_size;
  u_int16_t end_input = clear_code + 1;

  // Init Dictionary
  vector<vector<int>> dict;
	for (size_t i = 0; i < (*used_ct)->size(); ++i) {
		dict.push_back(vector<int>());
		dict[i].push_back(i);
	}
	dict.push_back(vector<int>());
	dict[dict.size()-1].push_back(clear_code);
	dict.push_back(vector<int>());
	dict[dict.size()-1].push_back(end_input);

  vector<int> phrase;
	vector<int> tmp;
	size_t idx = 0;
	size_t original_size = dict.size();
  u_int16_t lzw_size = lzw_min_code_size+1;
  u_int8_t mask = 1;
  u_int32_t counter = 0;
	while (counter < data.size()){
     // Read the code
    idx = 0;
		// if (idx_size > MAX_LZW_SIZE) idx_size = MAX_LZW_SIZE;
    for(u_int8_t i = 0; i < lzw_size; i++) {
      u_int8_t bit = (data[counter] & mask) ? 1 : 0;
      idx = idx | (bit << i);
      mask = mask << 1;
      if (!mask) {
        counter++;
        mask = 1;
      }
    }

		if (idx < dict.size()) {
			if (dict[idx][0] == clear_code) { dict.resize(original_size); tmp.clear(); lzw_size++; continue; }
			if (dict[idx][0] == end_input) break;
			phrase = dict[idx];

			for (size_t i = 0; i < phrase.size(); ++i)
				image_data->push_back(phrase[i]);

			vector<int> v = tmp;
			v.push_back(phrase[0]);
			add_to_dic(dict, v, original_size);
		} else {
			phrase = tmp;
			phrase.push_back(tmp[0]);

			for (size_t i = 0; i < phrase.size(); ++i)
				image_data->push_back(phrase[i]);

			add_to_dic(dict, phrase, original_size);
		}

		tmp = phrase;
	}
}

/**
 * Process the text extension
 * @param file input
 */
void parse_text_ext(FILE * file) {
  // Skip it
  get_block_data(file);
}

/**
 * Process the comments extension
 * @param file input
 */
void parse_comment_ext(FILE * file) {
  // Skip it
  get_block_data(file);
}

/**
 * Parse data from the graphic extension
 * @param  file input
 * @return      graphic extension structure
 */
tGraphicExt parse_graphic_ext(FILE * file) {
  tGraphicExt data;
  // Skip first byte telling the extension size
  get_byte(file);

  // Unpack data
  data.packed_field = get_byte(file);
  data.delay_time = get_byte(file) | (get_byte(file) << BYTE);
  data.transparent_color_idx = get_byte(file);

  // TERMINATOR
  get_byte(file);
  return data;
}

/**
 * Parse the application extension
 * @param file input
 */
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

/**
 * Parse the content of whole GIF file and distribute parsing on beach block
 * @param file  input
 * @param gct   global color table
 */
vector<u_int8_t> parse_gif_content(FILE * file, vector<tRGB> * gct, vector<tRGB> ** used_ct) {
  vector<u_int8_t> image;

  // Read separators until end
  u_int8_t sep;
  while((sep = get_byte(file)) != TRAILER) {
    // Decide if it is IMAGE_BLOCK or extension
    tGraphicExt g_ext = {};
    switch(sep) {
      case IMAGE_BLOCK:
        parse_image_block(file, &image, gct, &g_ext, used_ct);
        break;
      case EXTENSION_BLOCK: {
        // Parse extensions separately
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

/**
 * Save the BMP image
 * @param file output
 * @param info GIF header which hold important data
 * @param data pixel data
 */
void dump_bmp(FILE * file, tGIFInfo info, vector<u_int8_t> * data, vector<tRGB> * ct) {
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
  bytes = BMP_HEADER_OFFSET+BMP_HEADER_SIZE;
	fwrite(&bytes, sizeof(u_int32_t), 1, file);
  // Header size
  bytes = BMP_HEADER_OFFSET;
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
  bytes = data->size();
	fwrite(&bytes, sizeof(u_int32_t), 1, file);
  u_int32_t zero[4] = {BMP_RES, BMP_RES, BMP_COLORS, BMP_COLORS};
	fwrite(zero, sizeof(u_int32_t), 4, file);

  // Flush data
  for (size_t i = 1; i <= info.height; ++i) {
		for (size_t j = 0; j < info.width; ++j) {
			u_int16_t idx = data->at((info.height - i)*info.width + j);
      tRGB color = ct->at(idx);
			fwrite(&color.b, sizeof(u_int8_t), 1, file);
      fwrite(&color.g, sizeof(u_int8_t), 1, file);
      fwrite(&color.r, sizeof(u_int8_t), 1, file);
		}

    // Align file
		size_t w = (3 * info.width) & 0x3;
		for (size_t j = 0; j < 4 - w && w != 0; ++j) {
			uint8_t p = 0x00;
			fwrite(&p, 1, 1, file);
		}
	}
}

/**
 * Convert GIF to BMP
 * @param  gif2bmp    statistics
 * @param  inputFile  GIF file
 * @param  outputFile BMP file
 * @return            EXIT_SUCCESS if everything is fine
 */
int gif2bmp(tGIF2BMP *gif2bmp, FILE *inputFile, FILE *outputFile) {
  // Verify the inputFile is a GIF
  if (!verify_gif_signature(inputFile)) {
    eprintf("Not a valid GIF signature.\n");
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
  vector<tRGB> * ct;
  vector<u_int8_t> image = parse_gif_content(inputFile, &global_color_table, &ct);

  // Match the end of file
  gif2bmp->gifSize = ftell(inputFile);
  fseek(inputFile, 0, SEEK_END);
  if (gif2bmp->gifSize != ftell(inputFile)){
    eprintf("Unexpected file size (analyzed size doesn't match with real size)\n");
    return EXIT_FAILURE;
  }
  gif2bmp->bmpSize = ftell(outputFile);

  dump_bmp(outputFile, info, &image, ct);

  return EXIT_SUCCESS;
}