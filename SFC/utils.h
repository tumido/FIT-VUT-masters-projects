#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#ifndef UTILS_H
#define UTILS_H

namespace utils {
  inline double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
  }
  inline double uniform(double around) {
    return -around + (rand() / (RAND_MAX + 1.0) * (2 * around));
  }
  inline int binomial(double sample) {
    if(sample < 0 || sample > 1) return 0;

    if (sample > rand() / (RAND_MAX + 1.0))
      return 1;
    return 0;
  }
  template<class T> inline std::string print_array(T * data, size_t length) {
    std::string output = "[";
    for(int x = 0; x < length; x++) output += std::to_string(data[x]) + ", ";
    output.erase(output.end()-2, output.end());
    output += "]";
    return output;
  }
  template<class T> inline void read_array(std::stringstream & ss, T * data, size_t size) {
    for (int i=0; i<size; i++) {
      data[i] = 0;
      if (!ss.good()) continue;

      std::string substr;
      std::getline( ss, substr, ',' );
      std::istringstream value(substr);
      value >> data[i] >> std::ws;
    }
  }
  template<class T> inline void read_array(T * data, size_t size) {
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    read_array(ss, data, size);
  }

  template<class T> inline void read_value(T & data) {
    std::string input;
    std::getline(std::cin, input);
    std::istringstream value(input);
    value >> data;
  }

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
  inline bool check_file(std::string filename) {
    std::ifstream infile(filename);
    return infile.good();
  }
}
#endif
