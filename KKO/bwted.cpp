#include "bwted.h"

int BWTEncoding(tBWTED *bwted, FILE *inputFile, FILE *outputFile) {
  bwted->uncodedSize = 0;
  size_t length = 0, original_index = 0;

  char buffer[BLOCK_SIZE+1];
  char mtf_buffer[CHAR_VALUES];

  char tmp;
  positioned_string suffixes[BLOCK_SIZE+1];

  std::string bwt_string, mtf_string, rle_string;
  bwt_string.reserve(BLOCK_SIZE+1);
  mtf_string.reserve(BLOCK_SIZE+1);

  u_int8_t rle_counter;

  while ((length = fread(&buffer, sizeof(char), BLOCK_SIZE, inputFile)) != 0) {
    // Update log counter
    bwted->uncodedSize += length;

    buffer[length] = '\0';
    bwt_string = mtf_string = rle_string = "";

    // Build suffix array
    for (size_t i = 0; i < length; i++) {
      suffixes[i].position = i;
      suffixes[i].string = &buffer[i];
    }
    std::sort(suffixes, suffixes+length, [](const positioned_string a, const positioned_string b)
      { return (std::string) a.string < (std::string) b.string; });

    // Create BWT string from the suffixes
    for (size_t i = 0; i < length; i++) {
      if (suffixes[i].position == 0) {
        bwt_string[i] = buffer[length-1];
        original_index = i;
      } else {
        bwt_string[i] = buffer[suffixes[i].position-1];
      }
    }

    // MTF encoding
    for (size_t i = 0; i < CHAR_VALUES; i++) mtf_buffer[i] = (char) i;
    size_t ab_idx;
    for (size_t i = 0; i < length; i++) {
      // Find the letter index in alphabetical map
      for (ab_idx = 0; ab_idx < CHAR_VALUES; ab_idx++)
        if (bwt_string[i] == mtf_buffer[ab_idx]) break;

      // Remember the index in mtf_string
      tmp = bwt_string[i];
      mtf_string[i] = ab_idx;
      // Reorder mtf_buffer moving the letter to front
      for (size_t i = ab_idx; i > 0; i--) mtf_buffer[i] = mtf_buffer[i-1];
      mtf_buffer[0] = tmp;

    }

    // RLE-0 encoding
    for (size_t i = 0; i < length; i++) {
      // Take letter and count the same consecutive
      rle_counter = 1;
      tmp = mtf_string[i];
      while ((i+1 < length) && mtf_string[i] == mtf_string[i+1]) { rle_counter++; i++; }

      // Convert:
      // x      -> x
      // xx     -> xx0
      // xxxxxx -> xx4
      if (rle_counter == 1) {
        rle_string.push_back(tmp);
      } else {
        rle_string.push_back(tmp);
        rle_string.push_back(tmp);
        rle_string.push_back((char) rle_counter);
      }
    }

    // Separate chunks by delimiter (ASCII 255 by choice)
    rle_string.push_back((char) CHAR_VALUES-1);

    // Write to output file
    fwrite(&original_index, sizeof(original_index), 1, outputFile);
    fwrite(rle_string.c_str(), sizeof(char), rle_string.length(), outputFile);

    // Update log
    bwted->codedSize += rle_string.length() + sizeof(char);

    // Null buffers
    memset(buffer, 0, sizeof(char)*(BLOCK_SIZE+1));
    memset(mtf_buffer, 0, sizeof(char)*(CHAR_VALUES+1));
  }

  return 0;
}

int BWTDecoding(tBWTED *ahed, FILE *inputFile, FILE *outputFile) {

  return 0;
}

void printDebug(const std::string & header, const char * msg, const size_t length) {
  const std::string red("\033[0;31m");
  const std::string reset("\033[0m");
  std::cout << red << "DEBUG: " << reset << header << std::endl;
  for (size_t i = 0; i < length; i++ ) printf("%c", msg[i] != '\0' ? msg[i] : '_');
  std::cout << std::endl;
}
