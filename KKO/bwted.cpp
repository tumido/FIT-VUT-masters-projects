#include "bwted.h"

int BWTEncoding(tBWTED *bwted, FILE *inputFile, FILE *outputFile) {
  bwted->uncodedSize = bwted->codedSize = 0;
  size_t length = 0;
  u_int32_t original_index = 0;

  char buffer[BLOCK_SIZE];
  char mtf_buffer[CHAR_VALUES];

  char tmp;
  positioned_string suffixes[BLOCK_SIZE];

  std::string bwt_string, mtf_string, rle_string;
  bwt_string.reserve(BLOCK_SIZE+1);
  mtf_string.reserve(BLOCK_SIZE+1);

  u_int8_t rle_counter;

  while ((length = fread(&buffer, sizeof(char), BLOCK_SIZE, inputFile)) != 0) {
    // Update log counter
    bwted->uncodedSize += length;

    printDebug("Original", buffer, length);

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
    printDebug("BWT encoded", bwt_string.c_str(), length);

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
    printDebug("MTF encoded", mtf_string.c_str(), length);

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
    printDebug("RLE encoded", rle_string.c_str(), rle_string.length());

    // Separate chunks by delimiter (ASCII 255 by choice)
    rle_string.push_back(DELIMITER);

    // Write to output file
    fwrite(&original_index, sizeof(u_int32_t), 1, outputFile);
    fwrite(rle_string.c_str(), sizeof(char), rle_string.length(), outputFile);

    // Update log
    bwted->codedSize += rle_string.length() + sizeof(original_index);

    // Null buffers
    memset(buffer, 0, sizeof(char)*(BLOCK_SIZE+1));
    memset(mtf_buffer, 0, sizeof(char)*(CHAR_VALUES+1));
  }

  return 0;
}

int BWTDecoding(tBWTED *bwted, FILE *inputFile, FILE *outputFile) {
  bwted->codedSize = bwted->uncodedSize = 0;
  size_t length = 0;
  u_int32_t original_index = 0;

  char buffer[BLOCK_SIZE+1];
  char mtf_buffer[CHAR_VALUES];

  char tmp;
  positioned_string suffixes[BLOCK_SIZE+1];

  std::string bwt_string, mtf_string, original_string;
  bwt_string.reserve(BLOCK_SIZE+1);
  mtf_string.reserve(BLOCK_SIZE+1);

  while ((length = fread(&original_index, sizeof(u_int32_t), 1, inputFile)) != 0) {
    // Update log counter
    bwted->codedSize += length;

    bwt_string = mtf_string = original_string = "";

    // Read the block until delimiter is located
    length = 0;
    while ((tmp = fgetc(inputFile)) != DELIMITER) {
      buffer[length] = tmp;
      length++;
    }
    bwted->codedSize += length + sizeof(DELIMITER);
    printDebug("Coded", buffer, length);

    // RLE-0 decode
    for (size_t i = 0; i < length; i++) {
      if ((i+1 < length) && buffer[i] == buffer[i+1]) {
        for (u_int8_t rle_counter = buffer[i+2]; rle_counter > 0; rle_counter--) {
          mtf_string.push_back(buffer[i]);
        }
        i = i+2;
      } else {
        mtf_string.push_back(buffer[i]);
      }
    }
    length = mtf_string.length();
    printDebug("RLE decoded", mtf_string.c_str(), length);

    // MTF decode
    for (size_t i = 0; i < CHAR_VALUES; i++) mtf_buffer[i] = (char) i;
    size_t ab_idx;
    for (size_t i = 0; i < length; i++) {
      ab_idx = mtf_string[i];
      bwt_string[i] = mtf_buffer[ab_idx];
      for (size_t j = ab_idx; j > 0; j--) mtf_buffer[j] = mtf_buffer[j-1];
      mtf_buffer[0] = bwt_string[i];
    }
    printDebug("MTF decoded", bwt_string.c_str(), length);

    // Build suffix array
    for (size_t i = 0; i < length; i++) {
      suffixes[i].position = i;
      suffixes[i].string = &bwt_string[i];
    }
    std::stable_sort(suffixes, suffixes+length, [](const positioned_string a, const positioned_string b)
      { return a.string[0] < b.string[0]; });

    // BWT decode
    for (size_t i = 0; i < length; i++) {
      original_index = suffixes[original_index].position;
      original_string.push_back(bwt_string[original_index]);
    }
    //original_string[length-1] = '\0';
    printDebug("BWT decoded", original_string.c_str(), length);

    // Write to output file
    fwrite(original_string.c_str(), sizeof(char), original_string.length(), outputFile);

    // Update log
    bwted->uncodedSize += original_string.length();

    // Null buffers
    memset(buffer, 0, sizeof(char)*(BLOCK_SIZE+1));
    memset(mtf_buffer, 0, sizeof(char)*(CHAR_VALUES+1));
  }

  return 0;
}

void printDebug(const std::string & header, const char * msg, const size_t length) {
  const std::string red("\033[0;31m");
  const std::string reset("\033[0m");
  std::cout << red << "DEBUG: " << reset << header << std::endl;
  for (size_t i = 0; i < length; i++ ) {
    if (msg[i] >= 'a') printf("%c ", msg[i]);
    else printf("%d ", msg[i]);
  }
  std::cout << std::endl;
}
