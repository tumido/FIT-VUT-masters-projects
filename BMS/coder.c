/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  11/09/2016 08:22:18 PM
 *         Author:  Tomáš Coufal
 *          Login:  xcoufa09
 *        Project:  BMS - project 1 - RS code
 * =====================================================================
 */

/*
 * =====================================================================
 *   Reed Solomon - Coder - max. ratio 4:3
 * =====================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "rscode/ecc.h"

/*   Main
 * ---------------------------------------------------------------------
 * - read file <filename> which should be encoded
 * - compute parity (PARITY_SIZE) for each chunk of size CHUNK_SIZE
 * - store encoded data in <filename>.out
 */
int main(int argc, char const *argv[]) {
  // get the file name
  if (argc != 2) {
    print_err("No input file specified");
    return EXIT_FAILURE;
  }

  char buffer_read[CHUNK_SIZE];
  char buffer_write[CHUNK_SIZE + PARITY_SIZE];
  size_t size;
  FILE * input = fopen(argv[1], "rb");
  char * outname = build_filename(argv[1], ".out");
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

  // process the file by chunks or size CHUNK_SIZE
  while ((size = fread(buffer_read, 1, CHUNK_SIZE, input)) > 0) {
    // encode data into CHUNK_SIZE + PARITY_SIZE and store it
    encode_data((unsigned char *) buffer_read, size, (unsigned char *) buffer_write);

    // write data into the tempfiles
    interlace_write(buffer_write, size + PARITY_SIZE, &tmpfiles);

    // display the chunk before and after the encoding
    #ifdef DEBUG_LOGGING
    print_out("Bytes in", buffer_read, size, PARITY_SIZE);
    print_out("Bytes out", buffer_write, size, PARITY_SIZE);
    #endif
  }
  // join tempfiles
  interlace_join(&tmpfiles, output);

  // check for errors when using the io files
  if (ferror(input) || ferror(output)) {
    print_err("Error when proccessing a file");
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
