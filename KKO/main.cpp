#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "bwted.hpp"

#define LOGIN "xcoufa09"

enum Mode { ENCODE, DECODE, NONE };

int main(int argc, char ** argv) {
  FILE * infile, * outfile;
  std::string infilename, outfilename, logfilename;
  tBWTED operation_log = {0};
  int option;
  Mode mode = NONE;

  if (argc == 1) {
    std::cout << "bwted: missing operand" << std::endl;
    std::cout << "Try 'bwted -h' for more information" << std::endl;
    return EXIT_FAILURE;
  }

  // Parse arguments
  while ((option = getopt(argc, argv, "i:o:l:cxh")) != -1) {
    switch (option) {
      case 'i':
        infilename = optarg;
        break;
      case 'o':
        outfilename = optarg;
        break;
      case 'l':
        logfilename = optarg;
        break;
      case 'c':
      case 'x':
        if (mode != NONE) {
          std::cout << "bwted: multiple mode selection, aborting" << std::endl;
          std::cout << "Try 'bwted -h' for more information" << std::endl;
          return EXIT_FAILURE;
        }
        mode = option == 'c' ? ENCODE : DECODE;
        break;
      case 'h':
        std::cout << "Usage: bwted [-i INPUTFILE] [-o OUTPUTFILE] [-c | -x] [-h] [-l LOGFILE]" << std::endl;
        std::cout << "Provides encoding and decoding via Burrows-Wheeler transformation." << std::endl;
        std::cout << std::endl;
        std::cout << "Mode selection:" << std::endl;
        std::cout << "-c             Encoding mode (from plaintext)" << std::endl;
        std::cout << "-x             Decoding mode (extracts original data)" << std::endl;
        std::cout << "-h             Prints this message" << std::endl;
        std::cout << std::endl;
        std::cout << "Argument description:" << std::endl;
        std::cout << "-i INPUTFILE   Specify filename to read data from, if not set STDIN is used" << std::endl;
        std::cout << "-o OUTPUTFILE  Specify filename where the output is stored. Use STDOUT otherwise" << std::endl;
        std::cout << "-l LOGFILE     Set file where the log metrics are stored" << std::endl;
        return EXIT_SUCCESS;
      default:
        std::cout << "bwted: missing operand" << std::endl;
        std::cout << "Try 'bwted -h' for more information" << std::endl;
        return EXIT_FAILURE;
    }
  }

  // Set file descriptors
  infile  = infilename  != "" ? fopen(infilename.c_str(), "rb")  : stdin;
  outfile = outfilename != "" ? fopen(outfilename.c_str(), "wb") : stdout;

  if (mode == ENCODE)  BWTEncoding(&operation_log, infile, outfile);
  if (mode == DECODE)  BWTDecoding(&operation_log, infile, outfile);

  fclose(infile);
  fclose(outfile);

  if (logfilename != "") {
    std::ofstream logstream;
    logstream.open(logfilename);
    logstream << "login = " << LOGIN << std::endl;
    logstream << "uncodedSize = " << operation_log.uncodedSize << std::endl;
    logstream << "codedSize = " << operation_log.codedSize << std::endl;
    logstream.close();
  }


  return EXIT_SUCCESS;
}
