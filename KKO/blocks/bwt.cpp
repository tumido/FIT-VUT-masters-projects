#include "bwt.hpp"

void bwt::encode(char * buffer, size_t length, u_int32_t * index, std::string & output) {
  tPositionedString permutations[BLOCK_SIZE];

  // Build permutations array
  for (size_t i = 0; i < length; i++) {
    permutations[i].position = i;
    permutations[i].string = "";
    for (size_t j = 0; j < length; j++) permutations[i].string.push_back(buffer[(i+j) % length]);
  }
  std::stable_sort(permutations, permutations+length, [](const tPositionedString a, const tPositionedString b)
    { return a.string < b.string; });

  // Create BWT string from the permutations based on the index
  for (size_t i = 0; i < length; i++) {
    if (permutations[i].position == 0) {
      output[i] = buffer[length-1];
      *index = i;
    } else {
      output[i] = buffer[permutations[i].position-1];
    }
  }
}

void bwt::decode(std::string & input, size_t length, u_int32_t index, std::string & output) {
  tPositionedString permutations[BLOCK_SIZE];

  // Build suffix array
  for (size_t i = 0; i < length; i++) {
    permutations[i].position = i;
    permutations[i].string = &input[i];
  }
  std::stable_sort(permutations, permutations+length, [](const tPositionedString a, const tPositionedString b)
    { return a.string[0] < b.string[0]; });

  // Take index and decode from permutations
  for (size_t i = 0; i < length; i++) {
    index = permutations[index].position;
    output.push_back(input[index]);
  }
}
