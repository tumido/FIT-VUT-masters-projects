#include <stdio.h>
#include "bwted.h"

int main(int argc, char const ** argv) {
  FILE * infile, * outfile;

  tBWTED operation_log;

  BWTEncoding(&operation_log, stdin, stdout);

  return 0;
}
