#pragma once

#include <iostream>
#include <cstring>

#include "blocks/common.hpp"
#include "blocks/bwt.hpp"
#include "blocks/mtf.hpp"
#include "blocks/rle.hpp"
#include "blocks/shc.hpp"

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
