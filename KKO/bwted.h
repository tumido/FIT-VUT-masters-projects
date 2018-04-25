#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>

#define BLOCK_SIZE 10240
#define CHAR_VALUES 256
#define DELIMITER (char)CHAR_VALUES-1

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

/**
 * BTW encode
 * @param  bwted      encoding log struct
 * @param  inputFile  plain text input file
 * @param  outputFile encoded output file
 * @return            success 0, failure when encoding -1
 */
int BWTEncoding(tBWTED *bwted, FILE *inputFile, FILE *outputFile);

/**
 * BTW decode
 * @param  bwted      decoding log struct
 * @param  inputFile  encoded input file
 * @param  outputFile decoded plain text output file
 * @return            success 0, failure when encoding -1
 */
int BWTDecoding(tBWTED *ahed, FILE *inputFile, FILE *outputFile);

void printDebug(const std::string & header, const char * msg, const size_t length);
