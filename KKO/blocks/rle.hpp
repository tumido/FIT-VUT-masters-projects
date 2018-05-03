#pragma once

#include "common.hpp"

namespace rle {
  /**
   * Encode and compress via RLE
   *
   * Map data using this transformation:
   * x      -> x
   * xx     -> xx0
   * xxxxxx -> xx4
   *
   * @param input  String to compress
   * @param length Length of the input
   * @param output Compressed string
   */
  void encode(std::string & input, size_t length, std::string & output);
  /**
   * Decode from RLE
   *
   * Decode following pattern:
   * x   -> x
   * xx0 -> xx
   * xx4 -> xxxxxx
   * @param input  RLE encoded string
   * @param length Current string length, updated to the unpacked length
   * @param output Decodec message
   */
  void decode(std::string & input, size_t * length, std::string & output);
}
