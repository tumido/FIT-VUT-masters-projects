#include "rle.hpp"

void rle::encode(std::string & input, size_t length, std::string & output) {
  uint8_t rle_counter;
  char tmp;

  for (size_t i = 0; i < length; i++) {
    // Take letter and count the same consecutive
    rle_counter = 1;
    tmp = input[i];
    while ((i+1 < length) && input[i] == input[i+1]) { rle_counter++; i++; }

    // Convert
    if (rle_counter == 1) {
      output.push_back(tmp);
    } else {
      output.push_back(tmp);
      output.push_back(tmp);
      output.push_back((char) rle_counter);
    }
  }
}

void rle::decode(std::string & input, size_t * length, std::string & output) {
  for (size_t i = 0; i < input.length(); i++) {
    if ((i+1 < input.length()) && input[i] == input[i+1]) {
      for (uint8_t rle_counter = input[i+2]; rle_counter > 0; rle_counter--) {
        output.push_back(input[i]);
      }
      i = i+2;
    } else {
      output.push_back(input[i]);
    }
  }
  *length = output.length();
}
