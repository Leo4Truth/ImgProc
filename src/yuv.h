#ifndef _YUV_H_
#define _YUV_H_

#include "bmp.h"

typedef struct YUV
{
    double Y;
    double U;
    double V;
} YUV, *PYUV;

typedef struct YUVFILE
{
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    PYUV yuv;
} YUVFILE, *PYUVFILE;

PYUVFILE bmp2yuv(PBMP pbmp);
PBMP yuv2bmp(PYUVFILE pyuvfile);

#endif // yuv.h