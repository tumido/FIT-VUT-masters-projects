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
 *   Input/Output module
 * =====================================================================
 */
#include "io.h"
#define BUFFER_SIZE 1024

/*   Print chunk
 * ---------------------------------------------------------------------
 * - take a chunk data and display it's content as HEX
 * - display the parity bits separately
 */
void print_out(const char * msg, char byte_array[], int content, int parity) {
  printf("%-15s", msg);

  for (int i = 0; i < content + parity; i++) {
    printf("%2x ", (unsigned char)byte_array[i]);

    if (i == content - 1)
      printf("\tparity: ");
  }

  printf("\n");
}

/*   Error output
 * ---------------------------------------------------------------------
 * - place the message on the stderr
 */
void print_err(const char * msg) {
  fprintf(stderr, "\x1B[31m%s\033[0m\n", msg);
}

/*   Statistics
 * ---------------------------------------------------------------------
 * - display values specific to the en/decoding
 * - preset chunk size, amount of parity bits and file sizes
 */
void print_stats(FILE * input, FILE * output) {
  printf("\nStats:\n");
  printf("Chunks size:               %d\n", CHUNK_SIZE);
  printf("Added parity bits:         %d\n", PARITY_SIZE);
  printf("Reading from file of size: %.ld B\n", ftell(input));
  printf("Output file size is:       %.ld B\n", ftell(output));
}

/*   Concatenate strings
 * ---------------------------------------------------------------------
 * - add the extension the the filename
 */
char * build_filename(const char * filename, const char * extension) {
  size_t length = strlen(filename) +  strlen(extension);
  char * name = malloc((length + 1) * sizeof(char));
  if (name == NULL) {
    print_err("Unable to allocate memory.");
    return NULL;
  }

  strcpy(name, filename);
  strcat(name, extension);
  name[length] = '\0';
  return name;
}

size_t get_size(FILE * file) {
  size_t backup = ftell(file);
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, backup, SEEK_SET);
  return size;
}

void interlace_write(char * buffer, size_t size, interlace * files) {
  for (int i = 0; i < size; i++) {
    fputc(buffer[i], files->iterate[i % TEMP_FILES]);
  }
}

size_t interlace_read(char * buffer, size_t size, interlace * files) {
  size_t read = 0;
  size_t chunk = 0;
  for (int i = 0; i < size; i++) {
    read = fread(buffer + i, 1, 1, files->iterate[i % TEMP_FILES]);
    chunk = chunk + read;
    if (read == 0) break;
  }
  return chunk;
}

void interlace_open(interlace * files) {
  for (int i = 0; i < TEMP_FILES; i++) {
    files->iterate[i] = tmpfile();
  }
  return;
}

void interlace_close(interlace * files) {
  for (int i = 0; i < TEMP_FILES; i++) {
    fclose(files->iterate[i]);
  }
  return;
}

void interlace_join(interlace * files, FILE * output) {
  size_t size;
  char buffer[BUFFER_SIZE];

  for (int i = 0; i < TEMP_FILES; i++) {
    fseek(files->iterate[i], 0, SEEK_SET);
    while ((size = fread(buffer, 1, BUFFER_SIZE, files->iterate[i])) > 0) {
      fwrite(buffer, 1, size, output);
    }
  }
  return;
}

void interlace_split(interlace * files, FILE * input) {
  size_t whole = get_size(input);
  size_t part = whole / (float) TEMP_FILES + 1;
  char byte;
  int i = 0;
  for (size_t pos = 0; pos <= whole; pos++) {
    fread(&byte, 1, 1, input);
    fwrite(&byte, 1, 1, files->iterate[i]);
    if (!(pos % part)) { i++; }
    if (i >= TEMP_FILES) {
      printf("%ld\n%ld\n", pos, ftell(input)); break; }
  }

  for (int i = 0; i < TEMP_FILES; i++) {
    fseek(files->iterate[i], 0, SEEK_SET);
  }
}
