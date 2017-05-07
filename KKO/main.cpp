#include <getopt.h>
#include <sys/types.h>
#include "gif2bmp.h"

using namespace std;

void printHelp() {
  printf("Usage: gif2bmp [-i input_file] [-o output_file] [-l log_file] [-h]\n");
  printf("  -i input file name (GIF), default: stdin\n");
  printf("  -o output file name (BMP), default: stdout\n");
  printf("  -l specify log file name\n");
  printf("  -h print help\n");
}

int main(int argc, char *argv[]) {
  tGIF2BMP logging = {0, 0};
  FILE * in_f = stdin, * out_f = stdout, * log_f = NULL;

  // Parse arguments
  int arg;
  opterr = 0;
  while((arg = getopt(argc, argv, "i:o:l:h")) != -1) {
    switch(arg) {
      case 'i':
        if ((in_f = fopen(optarg, "r")) == NULL){
          eprintf("Unable to open input file '%s'", optarg)
          return EXIT_FAILURE;
        }
        break;
      case 'o':
        if ((out_f = fopen(optarg, "w")) == NULL){
          eprintf("Unable to open output file '%s'", optarg)
          return EXIT_FAILURE;
        }
        break;
      case 'l':
        if ((log_f = fopen(optarg, "w")) == NULL){
          eprintf("Unable to open log file '%s'", optarg)
          return EXIT_FAILURE;
        }
        break;
      case 'h':
        printHelp();
        return EXIT_SUCCESS;
      case '?':
        if(optopt == 'i' || optopt == 'o' || optopt == 'l')
          eprintf("Option -%c requires an argument.\n", optopt)
        else if(isprint(optopt))
          eprintf("Unknown option '-%c'.\n", optopt)
        else
          eprintf("Unknown option character '\\x%x'.\n", optopt);
        printHelp();
        return EXIT_FAILURE;
      default:
        eprintf("Unknown program argument error.\n");
        printHelp();
        return EXIT_FAILURE;
    }
  }

  gif2bmp(&logging, in_f, out_f);

  if(log_f != NULL) {
    fprintf(log_f, "login = xcoufa09\n");
    fprintf(log_f, "uncodedSize = %ld\n", logging.bmpSize);
    fprintf(log_f, "codedSize = %ld\n", logging.gifSize);
    fclose(log_f);
  }

  if(in_f != stdin)
    fclose(in_f);

  if(out_f != stdout)
    fclose(out_f);

  return EXIT_SUCCESS;
}