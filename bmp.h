#include <stdint.h>

// Data types defined in microsoft  windows api used for bitmap

typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef uint16_t WORD;

/*
Taken from
https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
 */

typedef struct {
  WORD bfType;
  DWORD bfSize;
  WORD bfReserver1;
  WORD bfReserver2;
  DWORD bfOffBits;
} __attribute__((__packed__)) BITMAPFILEHEADER;

/*
Taken from
https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
 */

typedef struct {
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} __attribute__((__packed__)) BITMAPINFOHEADER;

// RGB color channels

typedef struct {
  BYTE Red;
  BYTE Green;
  BYTE Blue;
} __attribute__((__packed__)) RGBTRIPLE;
