#include "histogram.h"

pHistogram histogramFromGrayscaleBmp(PBMP grayscaleImg) {
    int size = grayscaleImg->bmih.biHeight * grayscaleImg->bmih.biWidth;

    pHistogram res = (pHistogram)malloc(sizeof(Histogram));
    res->N = size;
    res->n = (int *)malloc(sizeof(int) * 256);
    res->p = (double *)malloc(sizeof(double) * 256);
    res->s = (double *)malloc(sizeof(double) * 256);

    int i, j;
    
    for (j = 0; j < 256; j++) {
        res->p[j] = 0.0f;
        res->s[j] = 0.0f;
        res->n[j] = 0;
    }

    for (i = 0; i < size; i++) {
        for (j = 0; j < 256; j++) {
            if (grayscaleImg->bgr[i].B == j) {
                res->n[j]++;
                break;
            }
        }
    }

    for (j = 0; j < 256; j++) {
        res->p[j] = (double)res->n[j] / size;
        int sum = 0;
        for (i = 0; i <= j; i++)
            sum += res->n[i];
        res->s[j] = (double)sum / size;
    }

    printHistogram(res);

    return res;
}

PBMP imgHistoEqualize(PBMP img) {
    int size = img->bmih.biHeight * img->bmih.biWidth;

    PBMP res = garyscale(img);
    pHistogram histo = histogramFromGrayscaleBmp(res);
    
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < 256; j++) {
            if (res->bgr[i].B == j) {
                res->bgr[i].B = res->bgr[i].G = res->bgr[i].R = histo->s[j] * 255;
                break;
            }
        }
    }

    deleteHistogram(histo);

    return res;
}

pColorHistogram colorHistogramFromBmp(PBMP img) {
    int size = img->bmih.biHeight * img->bmih.biWidth;
    
    pColorHistogram res = (pColorHistogram)malloc(sizeof(ColorHistogram));
    res->histogramB = (pHistogram)malloc(sizeof(Histogram));
    res->histogramG = (pHistogram)malloc(sizeof(Histogram));
    res->histogramR = (pHistogram)malloc(sizeof(Histogram));
    res->histogramB->p = (double *)malloc(sizeof(double) * 256);
    res->histogramB->s = (double *)malloc(sizeof(double) * 256);
    res->histogramG->p = (double *)malloc(sizeof(double) * 256);
    res->histogramG->s = (double *)malloc(sizeof(double) * 256);
    res->histogramR->p = (double *)malloc(sizeof(double) * 256);
    res->histogramR->s = (double *)malloc(sizeof(double) * 256);
    res->histogramB->n = (int *)malloc(sizeof(int) * 256);
    res->histogramG->n = (int *)malloc(sizeof(int) * 256);
    res->histogramR->n = (int *)malloc(sizeof(int) * 256);
    res->histogramB->N = size;
    res->histogramB->N = size;
    res->histogramB->N = size;

    int i, j;

    for (j = 0; j < 256; j++) {
        res->histogramB->p[j] = 0.0f;
        res->histogramB->s[j] = 0.0f;
        res->histogramG->p[j] = 0.0f;
        res->histogramG->s[j] = 0.0f;
        res->histogramR->p[j] = 0.0f;
        res->histogramR->s[j] = 0.0f;
        res->histogramB->n[j] = 0;
        res->histogramG->n[j] = 0;
        res->histogramR->n[j] = 0;
    }

    for (i = 0; i < size; i++) {
        for (j = 0; j < 256; j++) {
            if (img->bgr[i].B == j) {
                res->histogramB->n[j]++;
                break;
            }
        }
        for (j = 0; j < 256; j++) {
            if (img->bgr[i].G == j) {
                res->histogramG->n[j]++;
                break;
            }
        }
        for (j = 0; j < 256; j++) {
            if (img->bgr[i].R == j) {
                res->histogramR->n[j]++;
                break;
            }
        }
    }

    for (j = 0; j < 256; j++) {
        res->histogramB->p[j] = (double)res->histogramB->n[j] / size;
        res->histogramG->p[j] = (double)res->histogramG->n[j] / size;
        res->histogramR->p[j] = (double)res->histogramR->n[j] / size;
        
        int sumB = 0;
        int sumG = 0;
        int sumR = 0;
        for (i = 0; i <= j; i++) {
            sumB += res->histogramB->n[i];
            sumG += res->histogramG->n[i];
            sumR += res->histogramR->n[i];
        }
        res->histogramB->s[j] = (double)sumB / size;
        res->histogramG->s[j] = (double)sumG / size;
        res->histogramR->s[j] = (double)sumR / size;
    }

    printHistogram(res->histogramB);
    printHistogram(res->histogramG);
    printHistogram(res->histogramR);

    return res;
}

PBMP colorImgHistoEqualizeRGB(PBMP img) {
    int size = img->bmih.biHeight * img->bmih.biWidth;

    PBMP res = (PBMP)malloc(sizeof(BMP));
    copyFileHeader(&res->bmfh, &img->bmfh);
    copyInfoHeader(&res->bmih, &img->bmih);
    res->bgr = (PBGR)malloc(sizeof(BGR) * size);

    pColorHistogram clrHisto = colorHistogramFromBmp(img);

    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < 256; j++) {
            if (img->bgr[i].B == j) {
                res->bgr[i].B = clrHisto->histogramB->s[j] * 255;
                break;
            }
        }
        for (j = 0; j < 256; j++) {
            if (img->bgr[i].G == j) {
                res->bgr[i].G = clrHisto->histogramG->s[j] * 255;
                break;
            }
        }
        for (j = 0; j < 256; j++) {
            if (img->bgr[i].R == j) {
                res->bgr[i].R = clrHisto->histogramR->s[j] * 255;
                break;
            }
        }
    }

    deleteColorHistogram(clrHisto);

    return res;
}

PBMP colorImgHistoEqualizeY(PBMP img) {
    int size = img->bmih.biHeight * img->bmih.biWidth;

    PYUVFILE yuv = bmp2yuv(img);
    PBMP res = garyscale(img);
    pHistogram histo = histogramFromGrayscaleBmp(res);

    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < 256; j++) {
            if (res->bgr[i].B == j) {
                res->bgr[i].B = histo->s[j] * 255;
                break;
            }
        }
    }

    for (i = 0; i < size; i++)
        yuv->yuv[i].Y = (double)res->bgr[i].B;

    PBMP tmp = res;
    res = yuv2bmp(yuv);
    
    free(tmp->bgr);
    free(tmp);

    free(yuv->yuv);
    free(yuv);
    
    deleteHistogram(histo);

    return res;
}

void deleteHistogram(pHistogram histo) {
    free(histo->p);
    free(histo->s);
    free(histo);
}

void deleteColorHistogram(pColorHistogram clrHisto) {
    deleteHistogram(clrHisto->histogramB);
    deleteHistogram(clrHisto->histogramG);
    deleteHistogram(clrHisto->histogramR);
}

void printHistogram(pHistogram histo) {
    FILE *fp = fopen("../output.txt", "a+");
    int i, j;
    for (i = 0; i < 256; i++)
        fprintf(fp, "n[%d] = %d, p[%d] = %lf, s[%d] = %lf.\n",
                i, histo->n[i], i, histo->p[i], i, histo->s[i]);
    fprintf(fp, "\n\n");
    fclose(fp);
}