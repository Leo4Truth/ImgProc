#ifndef _BINARIZATION_H_
#define _BINARIZATION_H_

#include "bmp.h"
#include "yuv.h"

unsigned char OTSU_threshold(int *grayscaleHistogram, int pixel_total)
{
    unsigned int sumB = 0;
    unsigned int sum1 = 0;
    float wB = 0.0f;
    float wF = 0.0f;
    float mF = 0.0f;
    float max_var = 0.0f;
    float inter_var = 0.0f;
    unsigned char threshold = 0;
    unsigned short index_histo = 0;

    for (index_histo = 1; index_histo < 256; ++index_histo)
        sum1 += index_histo * grayscaleHistogram[index_histo];

    for (index_histo = 1; index_histo < 256; ++index_histo)
    {
        wB = wB + grayscaleHistogram[index_histo];
        wF = pixel_total - wB;
        if (wB == 0 || wF == 0)
            continue;
        sumB = sumB + index_histo * grayscaleHistogram[index_histo];
        mF = (sum1 - sumB) / wF;
        inter_var = wB * wF * ((sumB / wB) - mF) * ((sumB / wB) - mF);
        if (inter_var >= max_var)
        {
            threshold = index_histo;
            max_var = inter_var;
        }
    }

    return threshold;
}

PBMP binarize(PBMP origin) {
    PYUVFILE pyuvfile = bmp2yuv(origin);
    int size = origin->bmih.biHeight * origin->bmih.biWidth;
    PBMP res = (PBMP)malloc(sizeof(BMP));
    res->bgr = (PBGR)malloc(sizeof(BGR) * size);
    int i;
    int resscaleHistogram[256] = { 0 };
    unsigned char threshold = 0;
    
    copyFileHeader(&res->bmfh, &origin->bmfh);
    copyInfoHeader(&res->bmih, &origin->bmih);
    for (i = 0; i < size; i++) {
        res->bgr[i].B = res->bgr[i].G = res->bgr[i].R = (unsigned char)pyuvfile->yuv[i].Y;
        resscaleHistogram[res->bgr[i].B]++;
    }
    saveBmp("..\\img_output/grayscale.bmp", res);
    printf("grayscaled.\n");

    threshold = OTSU_threshold(resscaleHistogram, size);
    for (i = 0; i < size; i++) {
        if (res->bgr[i].B < threshold)
            res->bgr[i].B = res->bgr[i].G = res->bgr[i].R = 0;
        else
            res->bgr[i].B = res->bgr[i].G = res->bgr[i].R = 255;
    }

    return res;
}

#endif // bianrization.h