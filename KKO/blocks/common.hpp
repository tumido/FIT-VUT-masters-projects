#pragma once

#include <sys/types.h>
#include <string>

#define BLOCK_SIZE 10000
#define DELIMITER 0xFF

/**
 * BTW encoding/decoding log report
 */
typedef struct {
  int64_t uncodedSize;
  int64_t codedSize;
} tBWTED;

typedef struct {
  u_int32_t position;
  std::string string;
} tPositionedString;

typedef struct {
  char symbol;
  u_int32_t count;
} tFreqSymbol;
