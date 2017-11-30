#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>

#pragma once

namespace utils {
  /**
   * Calcualte sigmoid
   */
  inline double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
  }

  /**
   * Get a random value in range
   * @param  around positive range boundary
   * @return        random number
   */
  inline double uniform(double around) {
    return -around + (rand() / (RAND_MAX + 1.0) * (2 * around));
  }

  /**
   * Compute activation based on probability
   * @param  sample probability
   * @return        binary activation
   */
  inline int binomial(double sample) {
    if (sample < 0 || sample > 1) return 0;
    if (sample > rand() / (RAND_MAX + 1.0)) return 1;
    return 0;
  }

  /**
   * Pretty print an C-like array of values
   * @param  data   Array of values
   * @param  length Size of the array
   * @return        String representation of the array
   */
  template<class T> inline std::string print_array(T * data, size_t length) {
    std::string output = "[";
    for(size_t x = 0; x < length; x++) output += std::to_string(data[x]) + ", ";
    output.erase(output.end()-2, output.end());
    output += "]";
    return output;
  }

  /**
   * Read array from stringstream
   * @param ss   Stringstream to read from
   * @param data Data buffer - array for storing values
   * @param size Array members to load
   */
  template<class T> inline void read_array(std::stringstream & ss, T * data, size_t size) {
    for (size_t i=0; i<size; i++) {
      data[i] = 0;
      if (!ss.good()) continue;

      std::string substr;
      std::getline( ss, substr, ',' );
      std::istringstream value(substr);
      value >> data[i] >> std::ws;
      if (data[i] != 0) data[i] = 1; // fix other input than 0 or 1
    }
  }


  /**
   * Read array from stdin
   * @param data Data buffer - array for storing values
   * @param size Array members to load
   */
  template<class T> inline void read_array(T * data, size_t size) {
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    read_array(ss, data, size);
  }

  /**
   * Read a value from stdin
   * @param data Data storage
   */
  template<class T> inline void read_value(T & data) {
    std::string input;
    std::getline(std::cin, input);
    std::istringstream value(input);
    value >> data;
  }

  /**
   * Open and parse CSV file
   * @param filename Name of file to open
   * @param size     Amount of data on each line
   */
  inline std::vector<int *> read_csv(std::string filename, size_t size) {
    std::ifstream file(filename);
    std::string line;
    std::vector<int *> samples;
    // Parse file into vector of samples
    while (std::getline(file, line)) {
      int * sample = new int[size];
      std::stringstream ss(line);
      read_array(ss, sample, size);
      samples.push_back(sample);
    }

    return samples;
  }

  /**
   * Check if file exists
   * @param  filename Name of the file
   * @return          Boolean value
   */
  inline bool check_file(std::string filename) {
    std::ifstream infile(filename);
    return infile.good();
  }

  /**
   * Print heading with separator
   * @param heading Heading text
   */
  inline void header(std::string heading) {
    std::cout << std::endl << heading << std::endl << std::string(80, '-') << std::endl;
  }

  /**
   * Initialize a value from user input
   * @param label Message displayed to user
   * @param data  Variable to which the value is stored
   */
  template<class T> inline void init_value(std::string label, T & data) {
    std::cout << label << " (default = "<< data << "): ";
    read_value(data);
  }
}
