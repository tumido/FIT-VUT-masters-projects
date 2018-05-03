#pragma once

#include "common.hpp"

#define CHAR_VALUES 256

namespace mtf {
  /**
   * Encode via Move-to-front
   * @param input  String to transform
   * @param length Correct length of the string
   * @param output Transformed string
   */
  void encode(std::string & input, size_t length, std::string & output);

  /**
   * Decode Move-to-front transformation
   * @param input  String which should be transformed
   * @param length Correct length of the message
   * @param output Decoded sequence
   */
  void decode(std::string & input, size_t length, std::string & output);
}
