#ifndef _GEOMETRIC_H
#define _GEOMETRIC_H

#include <math.h>

#include "bmp.h"

PBMP translate(PBMP pbmp, int offsetX, int offsetY);
PBMP rotate(PBMP pbmp, double theta, int flag);
PBMP scale(PBMP pbmp, double ratioX, double ratioY);
PBMP shearOnX(PBMP pbmp, double dx);
PBMP shearOnY(PBMP pbmp, double dy);
PBMP mirror(PBMP pbmp, int symmetryAxis);
void geometric_test();

#endif