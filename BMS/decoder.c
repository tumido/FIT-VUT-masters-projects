/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  11/09/2016 08:30:12 PM
 *         Author:  Tomáš Coufal
 *          Login:  xcoufa09
 *        Project:  BMS - project 1 - RS code
 * =====================================================================
 */

/*
 * =====================================================================
 *   Reed Solomon - Decoder - max. ratio 4:3
 * =====================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "rscode/ecc.h"


/*   Main
 * ---------------------------------------------------------------------
 * - read file <filename> which should be decoded
 * - process the file by chunks
 * - check if each chunk contains broken bytes
 * - if so, try to repare it form the parity
 * - store encoded data in <filename>.ok
 */
int main(int argc, char const *argv[]) {
  // get the file name
  if (argc != 2) {
    print_err("No input file specified");
    return EXIT_FAILURE;
  }

  char buffer[CHUNK_SIZE + PARITY_SIZE];
  size_t size;
  int err[16];
  FILE * input = fopen(argv[1], "rb");
  char * outname = build_filename(argv[1], ".ok");
  FILE * output = fopen(outname, "wb");

  // check correctly opened files
  if (input == NULL || output == NULL) {
    print_err("Unable to open file");
    return EXIT_FAILURE;
  }

  // init the RS code engine
  initialize_ecc();


  // intitialize interlacing
  interlace tmpfiles;
  interlace_open(&tmpfiles);
  // split the input file
  interlace_split(&tmpfiles, input);

  // process the file by chunks of size CHUNK_SIZE + PARITY_SIZE
  while ((size = interlace_read(buffer, CHUNK_SIZE + PARITY_SIZE, &tmpfiles)) > 0) {
    // display the chunk before decoding
    #ifdef DEBUG_LOGGING
    print_out("Bytes in", buffer, size - PARITY_SIZE, PARITY_SIZE);
    #endif

    // decode data from the chunk and check for errors
    decode_data((unsigned char *) buffer, size);
    if (check_syndrome () != 0) {
      // try to repair the chunk
      #ifdef DEBUG_LOGGING
      print_err("Errors detected!");
      #endif
      correct_errors_erasures ((unsigned char *) buffer, size, 0, err);
    }

    fwrite(buffer, 1, size - PARITY_SIZE, output);

    // display the chunk after decoding and repair
    #ifdef DEBUG_LOGGING
    print_out("Bytes out", buffer, size - PARITY_SIZE, PARITY_SIZE);
    #endif
  }

  if (ferror(input) || ferror(output)) {
    print_err("Error when processing a file");
    return EXIT_FAILURE;
  }

  // print overall statistics
  #ifdef DEBUG_LOGGING
  print_stats(input, output);
  #endif

  interlace_close(&tmpfiles);
  fclose(input);
  fclose(output);
  free(outname);
  return EXIT_SUCCESS;
}
