#pragma once

#include <vector>
#include <deque>
#include <algorithm>

#include "common.hpp"
#include "btree.hpp"

namespace shc {
  /**
   * Static Huffman encoder
   *
   * Uses binary tree to encode message by characters and it's frequency
   * The most likely characters in message have the shortest code
   * @param input  String to encode
   * @param length Length of the string
   * @param output Buffer where the encoded message is appended
   */
  void encode(std::string & input, size_t length, std::string & output);

  /**
   * Static Huffman decoder
   *
   * The "header" has to contain 1 BYTE of symbol and 2 BYTES of it's number of appearances.
   * Based on this table the exact b-tree used to encode the message is created.
   * This tree is then used to decode the "content".
   * @param header  String representation of the character frequency table
   * @param content Encoded message
   * @param output  Buffer where the decoded message is stored
   */
  void decode(std::string & header, std::string & content, std::string & output);
}
