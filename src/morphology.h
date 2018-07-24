#ifndef _MORPHOLOGY_H_
#define _MORPHOLOGY_H_

#include "bmp.h"

typedef struct StructureElement {
    unsigned char *binaryImg;
    int height, width;
    int centerX, centerY;
    int area;
} StructureElement, *pStructureElement;

typedef struct StructureElementOffset {
    int *XOffset;
    int *YOffset;
    int area;
} StructureElementOffset, *pStructureElementOffset;

pStructureElementOffset toOffset(pStructureElement pse);
PBMP dilate(PBMP A, pStructureElement B);
PBMP erode(PBMP A, pStructureElement B);
PBMP open(PBMP A, pStructureElement B);
PBMP close(PBMP A, pStructureElement B);
PBMP hitOrMissTransform(PBMP A, pStructureElement C, pStructureElement D);

#endif // morphology.h