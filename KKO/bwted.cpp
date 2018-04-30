#include "bwted.h"

/**
 * Encode via Burrows-Wheeler transformation
 * @param buffer Input text buffer
 * @param length Input text length
 * @param index  The correct permutation index is stored here
 * @param output Encoded string
 */
void encodeBWT(char * buffer, size_t length, u_int32_t * index, std::string & output) {
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

/**
 * Decode via Burrows-Wheeler transformation
 * @param input  String on encoded text
 * @param length Correct text length
 * @param index  Original permutation index
 * @param output Plaintext string returned
 */
void decodeBWT(std::string & input, size_t length, u_int32_t index, std::string & output) {
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

/**
 * Encode via Move-to-front
 * @param input  String to transform
 * @param length Correct length of the string
 * @param output Transformed string
 */
void encodeMTF(std::string & input, size_t length, std::string & output) {
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

/**
 * Decode Move-to-front transformation
 * @param input  String which should be transformed
 * @param length Correct length of the message
 * @param output Decoded sequence
 */
void decodeMTF(std::string & input, size_t length, std::string & output) {
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
void encodeRLE(std::string & input, size_t length, std::string & output) {
  u_int8_t rle_counter;
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
void decodeRLE(char * input, size_t * length, std::string & output) {
  for (size_t i = 0; i < *length; i++) {
    if ((i+1 < *length) && input[i] == input[i+1]) {
      for (u_int8_t rle_counter = input[i+2]; rle_counter > 0; rle_counter--) {
        output.push_back(input[i]);
      }
      i = i+2;
    } else {
      output.push_back(input[i]);
    }
  }
  *length = output.length();
}

btree * build_huff_tree(std::vector<std::pair<char, u_int16_t>>& freq_map) {
  // Build the B-tree
  std::deque<btree*> all_trees;
  for (auto it = freq_map.begin(); it != freq_map.end(); it++) {
    char letter = it->first;
    u_int32_t count = it->second;
    all_trees.push_back(new btree(letter, count));
  }
  // Insert special delimiter symbol
  all_trees.push_back(new btree(DELIMITER, 1));

  // Join trees until only one is formed
  while (all_trees.size() > 1) {
    btree * a = all_trees.front();
    all_trees.pop_front();
    btree * b = all_trees.front();
    all_trees.pop_front();

    all_trees.push_back(a->join_tree(b));

    // Sort vector of trees so the lower frequencies are first;
    std::sort(all_trees.begin(), all_trees.end(),
      [](btree * a, btree * b) {
        if (a->get_root()->count == b->get_root()->count) return a->get_root()->label < b->get_root()->label;
        return a->get_root()->count < b->get_root()->count;
      }
    );
  }

  all_trees.front()->codify_nodes();
  return all_trees.front();
}

void encodeHUFF(std::string & input, size_t length, std::string & output) {
  // Count absolute frequency
  std::vector<std::pair<char, u_int16_t>> freq_map;

  for (size_t i = 0; i < length; i++) {
    auto it = std::find_if( freq_map.begin(), freq_map.end(),
      [&input,&i](const std::pair<char, u_int16_t>& e){ return e.first == input[i];}
    );

    if (it == freq_map.end())
      freq_map.push_back(std::pair<char, u_int16_t>(input[i], 1));
    else
      it->second += 1;
  }

  // Sort (from lowest to max freq)
  std::sort(freq_map.begin(), freq_map.end(),
    [](const std::pair<char, u_int16_t>& e, const std::pair<char, u_int16_t>& f) {
      if (e.second == f.second) return e.first < f.first;
      return e.second < f.second;
    }
  );
  // Save the counts in output for B-tree reconstruction
  for (auto it = freq_map.begin(); it != freq_map.end(); it++) {
    output.push_back(it->first);
    output.push_back(it->second >> BYTE);
    output.push_back(it->second);
  }
  // Add 2 bytes delimiter (255 by choice)
  output.push_back(DELIMITER);
  output.push_back(DELIMITER);


  // Build the B-tree
  btree * tree = build_huff_tree(freq_map);

  // Encode input string
  u_int8_t counter = 0;
  u_int16_t new_byte = 0;
  char letter;
  for (size_t i = 0; i <= length; i++) {
    letter = (i == length) ? DELIMITER : input[i];
    node * leaf = tree->search(letter);
    // std::cout << input[i] << ": " << leaf->label << ", " << std::bitset<BYTE>(leaf->code) << " (" << std::bitset<BYTE>(leaf->mask) << ")" <<std::endl;
    new_byte <<= leaf->mask.count();
    new_byte |= (leaf->code & leaf->mask).to_ulong();
    counter += leaf->mask.count();
    // std::cout << "counter: " << unsigned(counter) <<std::endl;
    if (counter >= BYTE) {
      output.push_back(new_byte >> (counter - BYTE));
      // std::cout << std::bitset<BYTE>(new_byte >> (counter - BYTE)) << std::endl;
      new_byte = 0 | (new_byte & (leaf->mask >> (leaf->mask.count() + BYTE - counter)).to_ulong());
      counter -= BYTE;
    }
  }

  // Encode end
  if (counter < BYTE) {
    new_byte <<= (BYTE-counter);
    new_byte |= (DELIMITER >> counter);
    output.push_back(new_byte);
  }
  // std::cout << std::bitset<BYTE>(new_byte) << std::endl;

  // Add 3 bytes delimiter (255 by choice)
  output.push_back(DELIMITER);
  output.push_back(DELIMITER);
  output.push_back(DELIMITER);

  // for (auto it = all_trees.begin(); it != all_trees.end(); it++) {
  //   (*it)->print();
  // }
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
    bwt_string = mtf_string = rle_string = "";

    printDebug("Original", buffer, length);

    // BWT -> MTF -> RLE -> HUFF
    encodeBWT(buffer, length, &original_index, bwt_string);
    encodeMTF(bwt_string, length, mtf_string);
    encodeRLE(mtf_string, length, rle_string);
    encodeHUFF(rle_string, rle_string.length(), huff_string);

    printDebug("BWT encoded", bwt_string.c_str(), length);
    printDebug("MTF encoded", mtf_string.c_str(), length);
    printDebug("RLE encoded", rle_string.c_str(), rle_string.length());
    printDebug("HUF encoded", huff_string.c_str(), huff_string.length());

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

  char buffer[BLOCK_SIZE+1];
  char tmp;

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

    // RLE-0 -> MTF -> BWT
    decodeRLE(buffer, &length, mtf_string);
    decodeMTF(mtf_string, length, bwt_string);
    decodeBWT(bwt_string, length, original_index, original_string);

    // DEBUG print
    printDebug("RLE decoded", mtf_string.c_str(), length);
    printDebug("MTF decoded", bwt_string.c_str(), length);
    printDebug("BWT decoded", original_string.c_str(), length);

    // Write to output file
    fwrite(original_string.c_str(), sizeof(char), original_string.length(), outputFile);

    // Update log
    bwted->uncodedSize += original_string.length();

    // Null buffers
    memset(buffer, 0, sizeof(char)*(BLOCK_SIZE+1));
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
