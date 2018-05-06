#include <iostream>
#include "mul32x32_061.c"
#include "mul32x32_065.c"
#include "mul32x32_072.c"
#include "mul32x32_092.c"

inline float multiply(float a, float b) {
  uint32_t * a_ptr = reinterpret_cast<uint32_t*>(&a);
  uint32_t * b_ptr = reinterpret_cast<uint32_t*>(&b);
  return a*b;
  // uint64_t product = mul32x32_061(*a_ptr, *b_ptr);
  // uint64_t product = mul32x32_065(*a_ptr, *b_ptr);
  // uint64_t product = mul32x32_072(*a_ptr, *b_ptr);
  // uint64_t product = mul32x32_092(*a_ptr, *b_ptr);
  // return (float) product;
}
