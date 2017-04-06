/*
 * =====================================================================
 *        Version:  1.0
 *        Created:  11/09/2016 08:40:31 PM
 *         Author:  Tomáš Coufal
 *          Login:  xcoufa09
 *        Project:  BMS - project 1 - RS code
 * =====================================================================
 */

/*
 * =====================================================================
 *   Input/Output header file
 * =====================================================================
 */
#ifndef IO_INCLUDED
#define IO_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// set the size of chunk and anout of parity bites applied
// the values must be no lower then in 4:3 ratio and can't reach over 255
#define CHUNK_SIZE 146
#define PARITY_SIZE 109

#define TEMP_FILES 8
typedef union ilace_u {
  FILE * iterate[TEMP_FILES];
} interlace;

/*   Print chunk
 * ---------------------------------------------------------------------
 * - take a chunk data and display it's content as HEX
 * - display the parity bits separately
 */
void print_out(const char * msg, char byte_array[], int content, int parity);

/*   Error output
 * ---------------------------------------------------------------------
 * - place the message on the stderr
 */
void print_err(const char * msg);

/*   Statistics
 * ---------------------------------------------------------------------
 * - display values specific to the en/decoding
 * - preset chunk size, amount of parity bits and file sizes
 */
void print_stats(FILE * input, FILE * output);

/*   Concatenate strings
 * ---------------------------------------------------------------------
 * - add the extension the the filename
 */
char * build_filename(const char * filename, const char * extension);


// size_t get_size(const char * filename);
size_t get_size(FILE * file);
void interlace_write(char * buffer, size_t size, interlace * files);
size_t interlace_read(char * buffer, size_t size, interlace * files);
void interlace_open(interlace * files);
void interlace_close(interlace * files);
void interlace_join(interlace * files, FILE * output);
void interlace_split(interlace * files, FILE * input);
#endif
