#ifndef _FILTER_H_
#define _FILTER_H_

#include "bmp.h"

#define MAX_INT 2147483647
#define MIN_INT (-2147483647 - 1)

#define PI 3.1415926
#define SQRT_2PI 2.50663

typedef struct IntBGR {
    int B;
    int G;
    int R;
} IntBGR;

double distance2D(int x1, int y1, int x2, int y2);
double distance3D(int x1, int y1, int z1, int x2, int y2, int z2);
double RGBDistanceSquare3D(BGR bgr1, BGR bgr2);
double gaussian(int x, int sigma);

PBMP meanFilter(PBMP pbmp);
PBMP laplacianEnhance(PBMP pbmp);

PBMP bilateralFilter(PBMP pbmp, int winR, int sigmaS, int sigmaR);
PBMP bilateralFilterOpt(PBMP pbmp, int winR, int sigmaS, int sigmaR);

void filterTestForAssignment5();
void filterTestForAssignment6();

#endif // filter.h