#include "bmp.h"
#include "filter.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf(" Usage: ./filter [flag] infile outfile\n ");
    return 1;
  }

  opterr = 0; // external vairable for getopt() error handling
  char *filters = "gb";
  char filter = getopt(argc, argv, filters);
  if (filter == '?') {
    // Handle unknown option
    printf("Invalid option.\n");
    return 2;
  }

  // Check if multiple filters are given
  if (getopt(argc, argv, filters) == -1) {
    printf("Only one fileter allowed.\n");
    return 3;
  }

  FILE *inptr = fopen(argv[2], "rb");
  if (inptr == NULL) {
    printf("Unable to input file.\n");
    return 4;
  }

  FILE *outptr = fopen(argv[3], "wb");
  if (outptr == NULL) {
    printf("Unable to open output file.\n");
    fclose(inptr);
    return 5;
  }

  BITMAPFILEHEADER bf;
  fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

  BITMAPINFOHEADER bi;
  fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

  // Check the correct file format and specifications
  if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
      bi.biPlanes != 1 || bi.biBitCount != 24) {
    fclose(outptr);
    fclose(inptr);
    printf("Unsupported file format.\n");
    return 6;
  }

  int height = abs(bi.biHeight);
  int width = bi.biWidth;

  RGBTRIPLE(**image) = calloc(height, sizeof(RGBTRIPLE *));
  if (image == NULL) {
    printf("Memory allocation failed.\n");
    fclose(outptr);
    fclose(inptr);
    return 7;
  }

  for (int i = 0; i < height; i++) {
    image[i] = calloc(width, sizeof(RGBTRIPLE));
    if (image[i] == NULL) {
      printf("Memory allocation failed.\n");
      for (int j = 0; j < i; j++) {
        free(image[j]);
      }
      free(image);
      fclose(outptr);
      fclose(inptr);
      return 7;
    }
  }

  int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4);
  for (int i = 0; i < height; i++) {
    fread(image[i], sizeof(RGBTRIPLE), width, inptr);
    fseek(inptr, padding, SEEK_CUR);
  }

  switch (filter) {
  case 'g':
    grayscale(image, height, width);
    break;
  case 'b':
    blur(image, height, width);
  }

  fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
  fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

  for (int i = 0; i < height; i++) {
    fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);
    for (int j = 0; j < padding; j++) {
      fputc(0x00, outptr);
    }
  }

  for (int i = 0; i < height; i++) {
    free(image[i]);
  }
  free(image);

  fclose(inptr);
  fclose(outptr);
}
