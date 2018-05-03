#pragma once

#include <algorithm>
#include "common.hpp"

namespace bwt {
  /**
   * Encode via Burrows-Wheeler transformation
   * @param buffer Input text buffer
   * @param length Input text length
   * @param index  The correct permutation index is stored here
   * @param output Encoded string
   */
  void encode(char * buffer, size_t length, uint32_t * index, std::string & output);


  /**
   * Decode via Burrows-Wheeler transformation
   * @param input  String on encoded text
   * @param length Correct text length
   * @param index  Original permutation index
   * @param output Plaintext string returned
   */
  void decode(std::string & input, size_t length, uint32_t index, std::string & output);
}
