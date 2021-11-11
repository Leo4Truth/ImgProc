#ifndef _BMP_H_
#define _BMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned char BYTE;

// 14 bytes
#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER {
    WORD bfType; // 2 bytes, 16 bits
    DWORD bfSize; // 4 bytes, 32 bits
    WORD bfReserved1; // 2 bytes, 16 bits
    WORD bfReserved2; // 2 bytes, 16 bits
    DWORD bfOffBits;  // 4 bytes, 32 bits
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;
#pragma pack(pop)

// 40 bytes
#pragma pack(push, 1)
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize; // 4 bytes, 32 bits
    LONG biWidth; // 4 bytes, 32 bits
    LONG biHeight; // 4 bytes, 32 bits
    WORD biPlanes; // 2 bytes, 16 bits
    WORD biBitCount; // 2 bytes, 16 bits
    DWORD biCompression; // 4 bytes, 32 bits
    DWORD biSizeImage; // 4 bytes, 32 bits
    LONG biXPelsPerMeter; // 4 bytes, 32 bits
    LONG biYPelsPerMeter; // 4 bytes, 32 bits
    DWORD biClrUsed; // 4 bytes, 32 bits
    DWORD biClrImportant; // 4 bytes, 32 bits
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack(pop)

// 24位 真彩色图象不使用彩色板
#pragma pack(push, 1)
typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD, *PRGBQUAD;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct BGR {
    BYTE B;
    BYTE G;
    BYTE R;
} BGR, *PBGR;
#pragma pack(pop)

typedef struct BMP {
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    PBGR bgr;
} BMP, *PBMP;

int loadBmp(const char *file_path, PBMP pbmp);
int saveBmp(const char *file_path, PBMP pbmp);
int copyBmp(PBMP des, PBMP src);
int copyFileHeader(PBITMAPFILEHEADER destination, PBITMAPFILEHEADER source);
int copyInfoHeader(PBITMAPINFOHEADER destination, PBITMAPINFOHEADER source);
int printHeader(PBMP pbmp);
void deleteBMP(PBMP pbmp);

PBMP garyscale(PBMP A);

#endif // bmp.h
