#include "shc.hpp"

btree * build_huff_tree(std::vector<std::pair<char, uint16_t>>& freq_map) {
  // Build the B-tree
  std::deque<btree*> all_trees;
  for (auto it = freq_map.begin(); it != freq_map.end(); it++) {
    char letter = it->first;
    uint32_t count = it->second;
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
    delete b;

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

void shc::encode(std::string & input, size_t length, std::string & output) {
  // Count absolute frequency
  std::vector<std::pair<char, uint16_t>> freq_map;

  for (size_t i = 0; i < length; i++) {
    // Locate the symbol in frequency map
    auto it = std::find_if( freq_map.begin(), freq_map.end(),
      [&input,&i](const std::pair<char, uint16_t>& e){ return e.first == input[i];}
    );

    // Update value
    if (it == freq_map.end())
      freq_map.push_back(std::pair<char, uint16_t>(input[i], 1));
    else
      it->second += 1;
  }

  // Sort (from lowest to max freq)
  std::sort(freq_map.begin(), freq_map.end(),
    [](const std::pair<char, uint16_t>& e, const std::pair<char, uint16_t>& f) {
      if (e.second == f.second) return e.first < f.first;
      return e.second < f.second;
    }
  );
  // Save the counts in output for B-tree reconstruction
  for (auto it = freq_map.begin(); it != freq_map.end(); it++) {
    output.push_back(it->first);

    // NOTE: Guard for 1xFF on the last byte of header (decrement by one shouldn't be harmful)
    if (it+1 == freq_map.end() && (char)it->second == (char)DELIMITER) it->second--;

    output.push_back(it->second >> WORD);
    output.push_back(it->second);
  }

  // Add 2 bytes delimiter (255 by choice)
  output.push_back(DELIMITER);
  output.push_back(DELIMITER);


  // Build the B-tree
  btree * tree = build_huff_tree(freq_map);
  // tree->print();

  // Encode input string
  uint8_t counter = 0;
  uint32_t new_byte = 0;
  char letter;
  for (size_t i = 0; i <= length; i++) {
    // If this is the last round, encode DELIMITER character
    letter = (i == length) ? DELIMITER : input[i];

    // Find symbol in the tree
    node * leaf = tree->search(letter);

    // Update the new byte with encoded value
    new_byte <<= leaf->mask.count();
    new_byte |= (leaf->code & leaf->mask).to_ulong();

    // Guard for full byte
    counter += leaf->mask.count();
    // If the encoded buffer exceeded 1 byte, store the value in output vector and do so until the buffer is full below 1 byte
    while (counter >= WORD) {
      output.push_back(new_byte >> (counter - WORD));
      new_byte &= (leaf->mask >> (leaf->mask.count() + WORD - counter)).to_ulong();
      counter -= WORD;
    }
  }

  // Align last WORD
  if (counter < WORD) {
    new_byte <<= (WORD-counter);
    output.push_back(new_byte);
  }
  // Dealloc b-tree
  tree->destroy_tree();
  delete tree;

  // Add 3 bytes delimiter (255 by choice)
  output.push_back(DELIMITER);
  output.push_back(DELIMITER);
  output.push_back(DELIMITER);
}

void shc::decode(std::string & header, std::string & content, std::string & output) {
  std::vector<std::pair<char, uint16_t>> freq_map;

  // Build vector of frequencies from header
  for (size_t i = 0; i < header.length()-2; i+=3) {
    uint16_t freq = header[i+1] << WORD;
    freq |= std::bitset<WORD>(header[i+2]).to_ulong();
    freq_map.push_back(std::pair<char, uint16_t>(header[i],freq));
  }

  // Build tree
  btree * tree = build_huff_tree(freq_map);
  // tree->print();

  // Read content
  std::bitset<DWORD> code = 0;
  std::bitset<DWORD> mask = 0;
  size_t letter_counter = 0, bit_counter = 0;
  std::bitset<WORD> letter = content[0];

  while (true) {
    // Take one bit from input
    code = (code << 1) | std::bitset<DWORD>(letter[WORD-1]);
    letter <<= 1;
    mask <<= 1; mask[0] = 1;
    bit_counter++;
    // std::cout << code << " " << mask <<std::endl;

    // Find if the code means something
    node * decoded = tree->search(code, mask);
    if (decoded != NULL) {
      // If the code means DELIMITER, end the run
      if (*(decoded->label.c_str()) == (char)DELIMITER) break;

      // Remember and reset otherwise
      output.push_back(*(decoded->label.c_str()));
      // printDebug("Partial", output.c_str(), output.length());
      code = 0;
      mask = 0;
    }

    // If whole WORD is read, move to the next letter
    if (bit_counter == WORD) {
      bit_counter = 0;
      letter_counter++;
      letter = content[letter_counter];
    }
  }

  // Dealloc b-tree
  tree->destroy_tree();
  delete tree;
}
