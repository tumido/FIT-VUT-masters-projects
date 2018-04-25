#include <stdio.h>
#include "bwted.h"

int main(int argc, char const ** argv) {
  FILE * infile, * outfile;
  outfile = fopen("/tmp/bwted", "wb");

  tBWTED operation_log;

  BWTEncoding(&operation_log, stdin, outfile);
  std::cout << "Uncoded size: " << operation_log.uncodedSize << std::endl;
  std::cout << "Coded size:   " << operation_log.codedSize << std::endl;

  fclose(outfile);
  infile = fopen("/tmp/bwted", "rb");
  BWTDecoding(&operation_log, infile, stdout);
  std::cout << std::endl << "Uncoded size: " << operation_log.uncodedSize << std::endl;
  std::cout << "Coded size:   " << operation_log.codedSize << std::endl;

  return 0;
}
