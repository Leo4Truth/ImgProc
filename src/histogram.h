#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include "bmp.h"
#include "yuv.h"

typedef struct Histogram {
    int N;
    int *n;
    double *p;
    double *s;
} Histogram, *pHistogram;

typedef struct ColorHistogram {
    pHistogram histogramR;
    pHistogram histogramG;
    pHistogram histogramB;
} ColorHistogram, *pColorHistogram;

pHistogram histogramFromGrayscaleBmp(PBMP grayscaleImg);
PBMP imgHistoEqualize(PBMP img);

pColorHistogram colorHistogramFromBmp(PBMP img);
PBMP colorImgHistoEqualizeRGB(PBMP img);
//PBMP colorImgHistoEqualizeY(PBMP img);

void deleteHistogram(pHistogram histo);
void deleteColorHistogram(pColorHistogram clrHisto);

void printHistogram(pHistogram histo);

#endif // histogram.h