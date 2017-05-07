#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <bitset>
#include <utility>
#include <unordered_map>
#include <unistd.h>
#include <sys/types.h>

#define GIF_SIGNATURE "GIF89a"
#define GIF_SIGNATURE_LENGTH 6

#define BMP_SIGNATURE "BM"
#define BMP_HEADER_SIZE 14
#define BMP_HEADER_OFFSET 40
#define BMP_ALIGN 4

#define BYTE sizeof(u_int8_t)

#define COLOR_TABLE_ENABLED_OFFSET 7
#define GCT_SIZE_MASK 7
#define LCT_SIZE_MASK 15
#define LCT_INTERLACE_OFFSET 6

#define IMAGE_BLOCK 0x2c
#define EXTENSION_BLOCK 0x21
#define GRAPHIC_EXT 0xf9
#define COMMENT_EXT 0xfe
#define TEXT_EXT 0x01
#define APP_EXT 0xff
#define TERMINATOR 0
#define TRAILER 0x3b

#define APP_ID_SIZE 8
#define eprintf(...) fprintf(stderr, ##__VA_ARGS__);

#define MAX_LZW_SIZE 12

#define BMP_N_PLANES 1
#define BMP_BPP 24
#define BMP_COMPRESS 0
#define BMP_RES 0
#define BMP_COLORS 0

typedef struct sRGB {
  u_int8_t r, g, b;
  sRGB(u_int8_t r, u_int8_t g, u_int8_t b): r(r), g(g), b(b) {}
} tRGB;

typedef struct {
  u_int16_t width;
  u_int16_t height;
  u_int8_t gct_flags;
  u_int8_t bg_color;
  u_int8_t px_ratio;
  // u_int8_t bpp;
  // u_int16_t gctSize;
} tGIFInfo;

typedef struct {
  u_int8_t packed_field;
  u_int8_t delay_time;
  u_int8_t transparent_color_idx;
} tGraphicExt;

typedef struct {
  u_int64_t bmpSize;
  u_int64_t long gifSize;
} tGIF2BMP;

/**
 * Convert GIF to BMP
 * @param  gif2bmp    statistics
 * @param  inputFile  GIF file
 * @param  outputFile BMP file
 * @return            EXIT_SUCCESS if everything is fine
 */
int gif2bmp(tGIF2BMP *gif2bmp, FILE *inputFile, FILE *outputFile);