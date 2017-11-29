#include <math.h>

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
}
#endif
