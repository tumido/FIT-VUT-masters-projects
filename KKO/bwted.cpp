#include "bwted.hpp"

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


int BWTEncoding(tBWTED *bwted, FILE *inputFile, FILE *outputFile) {
  size_t length = 0;
  u_int32_t original_index = 0;

  char buffer[BLOCK_SIZE];
  std::string bwt_string, mtf_string, rle_string, huff_string;
  bwt_string.reserve(BLOCK_SIZE+1);
  mtf_string.reserve(BLOCK_SIZE+1);

  while ((length = fread(&buffer, sizeof(char), BLOCK_SIZE, inputFile)) != 0) {
    // Update log counter
    bwted->uncodedSize += length;
    bwt_string = mtf_string = rle_string = huff_string = "";
    original_index = 0;

    // printDebug("Original", buffer, length);

    // BWT -> MTF -> RLE -> HUFF
    bwt::encode(buffer, length, &original_index, bwt_string);
    mtf::encode(bwt_string, length, mtf_string);
    rle::encode(mtf_string, length, rle_string);
    shc::encode(rle_string, rle_string.length(), huff_string);

    // printDebug("BWT encoded", bwt_string.c_str(), length);
    // printDebug("MTF encoded", mtf_string.c_str(), length);
    // printDebug("RLE encoded", rle_string.c_str(), rle_string.length());
    // printDebug("HUF encoded", huff_string.c_str(), huff_string.length());

    // Write to output file
    fwrite(&original_index, sizeof(u_int32_t), 1, outputFile);
    fwrite(huff_string.c_str(), sizeof(char), huff_string.length(), outputFile);

    // Update log
    bwted->codedSize += huff_string.length() + sizeof(original_index);

    // Null buffers
    memset(buffer, 0, sizeof(char)*(BLOCK_SIZE+1));
  }

  return 0;
}

int BWTDecoding(tBWTED *bwted, FILE *inputFile, FILE *outputFile) {
  size_t length = 0;
  u_int32_t original_index = 0;

  char tmp, tmp_prev = '\0', tmp_prev2 = '\0';

  std::string huff_header, huff_string, rle_string, bwt_string, mtf_string, original_string;
  bwt_string.reserve(BLOCK_SIZE+1);
  mtf_string.reserve(BLOCK_SIZE+1);

  while ((length = fread(&original_index, sizeof(u_int32_t), 1, inputFile)) != 0) {
    // Update log counter
    bwted->codedSize += sizeof(u_int32_t);

    huff_header = huff_string = bwt_string = mtf_string = original_string = "";
    // printDebug("RLE partial", rle_string.c_str(), rle_string.length());
    rle_string = "";

    tmp_prev = '\0';
    // Read the block until 2 delimiters are located - header
    while (!((tmp = fgetc(inputFile)) == (char)DELIMITER && tmp_prev == (char)DELIMITER)) {
      // printDebug("partial", &tmp, 1);
      huff_header.push_back(tmp);
      tmp_prev = tmp;
    }
    huff_header.pop_back();
    bwted->codedSize += huff_header.length() + sizeof(char)*2;

    // Read the block until 3 delimiters are located - content
    tmp_prev = '\0', tmp_prev2 = '\0';
    while (!((tmp = fgetc(inputFile)) == (char)DELIMITER && tmp_prev == (char)DELIMITER && tmp_prev2 == (char)DELIMITER)) {
      huff_string.push_back(tmp);
      tmp_prev2 = tmp_prev;
      tmp_prev = tmp;
    }
    huff_string.pop_back();
    huff_string.pop_back();
    bwted->codedSize += huff_string.length() + sizeof(char)*3;

    // HUFF -> RLE-0 -> MTF -> BWT
    shc::decode(huff_header, huff_string, rle_string);
    rle::decode(rle_string, &length, mtf_string);
    mtf::decode(mtf_string, length, bwt_string);
    bwt::decode(bwt_string, length, original_index, original_string);

    // DEBUG print
    // printDebug("HUF decoded", rle_string.c_str(), rle_string.length());
    // printDebug("RLE decoded", mtf_string.c_str(), length);
    // printDebug("MTF decoded", bwt_string.c_str(), length);
    // printDebug("BWT decoded", original_string.c_str(), length);

    // Write to output file
    fwrite(original_string.c_str(), sizeof(char), original_string.length(), outputFile);

    // Update log
    bwted->uncodedSize += original_string.length();
  }

  return 0;
}
