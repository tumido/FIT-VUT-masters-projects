#include "mtf.hpp"

void mtf::encode(std::string & input, size_t length, std::string & output) {
  char mtf_buffer[CHAR_VALUES] = {0};
  char tmp;
  size_t ab_idx;

  for (size_t i = 0; i < CHAR_VALUES; i++) mtf_buffer[i] = (char) i;

  for (size_t i = 0; i < length; i++) {
    // Find the letter index in alphabetical map
    for (ab_idx = 0; ab_idx < CHAR_VALUES; ab_idx++)
      if (input[i] == mtf_buffer[ab_idx]) break;

    // Remember the index in output
    tmp = input[i];
    output[i] = ab_idx;

    // Reorder mtf_buffer moving the letter to front
    for (size_t i = ab_idx; i > 0; i--) mtf_buffer[i] = mtf_buffer[i-1];
    mtf_buffer[0] = tmp;
  }
}

void mtf::decode(std::string & input, size_t length, std::string & output) {
  char mtf_buffer[CHAR_VALUES] = {0};
  size_t ab_idx;

  for (size_t i = 0; i < CHAR_VALUES; i++) mtf_buffer[i] = (char) i;

  for (size_t i = 0; i < length; i++) {
    ab_idx = input[i];
    output[i] = mtf_buffer[ab_idx];
    for (size_t j = ab_idx; j > 0; j--) mtf_buffer[j] = mtf_buffer[j-1];
    mtf_buffer[0] = output[i];
  }
}
