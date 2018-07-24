#ifndef _LOGARITHMIC_H_
#define _LOGARITHMIC_H_

#include <math.h>

#include "bmp.h"
#include "yuv.h"

PBMP visibilityEnhancementByLogarithmicOperation(PBMP img) {
    int size = img->bmih.biHeight * img->bmih.biWidth;
    int i, j;

    PYUVFILE pyuvfile = bmp2yuv(img);
    
    const double maxY = (0.299 + 0.587 + 0.114) * 255;
    double Lmax = 0.0;
    for (i = 0; i < size; i++) {
        if (pyuvfile->yuv[i].Y > Lmax)
            Lmax = pyuvfile->yuv[i].Y;
    }
    Lmax /= maxY;

    for (i = 0; i < size; i++) {
        double Lw = pyuvfile->yuv[i].Y / maxY;
        pyuvfile->yuv[i].Y = log(Lw + 1.0) / log(Lmax + 1.0) * maxY;
    }

    PBMP res = yuv2bmp(pyuvfile);

    free(pyuvfile->yuv);
    free(pyuvfile);

    return res;
}

#endif // logarithmic.h
