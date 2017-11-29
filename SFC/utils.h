#include <math.h>
#include <string>

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
}
#endif
