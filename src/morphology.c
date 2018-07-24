#include "morphology.h"



pStructureElementOffset toOffset(pStructureElement pse) {
    pStructureElementOffset pseo = (pStructureElementOffset)malloc(sizeof(StructureElementOffset));
    pseo->XOffset = (int *)malloc(sizeof(int) * pse->area);
    pseo->YOffset = (int *)malloc(sizeof(int) * pse->area);

    int i, j;
    pseo->area = 0;
    for (i = 0; i < pse->height; i++) {
        for (j = 0; j < pse->width; j++) {
            if (pse->binaryImg[i * pse->width + j] == 0) {
                pseo->XOffset[pseo->area] = j - pse->centerX;
                pseo->YOffset[pseo->area++] = i - pse->centerY;
            }
        }
    }
    
    return pseo;
}

PBMP dilate(PBMP A, pStructureElement B){
    pStructureElementOffset pseo = toOffset(B);

    PBMP res = (PBMP)malloc(sizeof(BMP));
    res->bgr = NULL;
    copyBmp(res, A);

    int i, j, k;
    for (i = 0; i < res->bmih.biHeight; i++) {
        for (j = 0; j < res->bmih.biWidth; j++) {
            for (k = 0; k < pseo->area; k++) {
                int m, n;
                m = i + pseo->YOffset[k];
                n = j + pseo->XOffset[k];
                if (m >= 0 && m < res->bmih.biHeight &&
                    n >= 0 && n < res->bmih.biWidth &&
                    res->bgr[m * res->bmih.biWidth + n].B == 0 &&
                    res->bgr[i * res->bmih.biWidth + j].B == 255) {
                    res->bgr[i * res->bmih.biWidth + j].B = 1;
                    res->bgr[i * res->bmih.biWidth + j].G = 0;
                    res->bgr[i * res->bmih.biWidth + j].R = 0;
                    break;
                }
            }
        }
    }
    
    for (i = 0; i < res->bmih.biHeight; i++) {
        for (j = 0; j < res->bmih.biWidth; j++) {
            if (res->bgr[i * res->bmih.biWidth + j].B == 1)
                res->bgr[i * res->bmih.biWidth + j].B = 0;
        }
    }

    return res;
}

PBMP erode(PBMP A, pStructureElement B) {
    pStructureElementOffset pseo = toOffset(B);

    PBMP res = (PBMP)malloc(sizeof(BMP));
    res->bgr = NULL;
    copyBmp(res, A);

    int i, j, k;
    for (i = 0; i < res->bmih.biHeight; i++) {
        for (j = 0; j < res->bmih.biWidth; j++) {
            for (k = 0; k < pseo->area; k++) {
                int m, n;
                m = i + pseo->YOffset[k];
                n = j + pseo->XOffset[k];
                if (m >= 0 && m < res->bmih.biHeight &&
                    n >= 0 && n < res->bmih.biWidth &&
                    res->bgr[m * res->bmih.biWidth + n].B == 255 &&
                    res->bgr[i * res->bmih.biWidth + j].B == 0) {
                    res->bgr[i * res->bmih.biWidth + j].B = 254;
                    res->bgr[i * res->bmih.biWidth + j].G = 255;
                    res->bgr[i * res->bmih.biWidth + j].R = 255;
                }
            }
        }
    }

    for (i = 0; i < res->bmih.biHeight; i++) {
        for (j = 0; j < res->bmih.biWidth; j++) {
            if (res->bgr[i * res->bmih.biWidth + j].B == 254)
                res->bgr[i * res->bmih.biWidth + j].B = 255;
        }
    }

    return res;
}

PBMP open(PBMP A, pStructureElement B)
{
    pStructureElementOffset pseo = toOffset(B);
    PBMP res = erode(A, B);
    res = dilate(res, B);
    return res;
}

PBMP close(PBMP A, pStructureElement B)
{
    pStructureElementOffset pseo = toOffset(B);
    PBMP res = dilate(A, B);
    res = erode(res, B);
    return res;
}

// Not Complete
PBMP hitOrMissTransform(PBMP A, pStructureElement C, pStructureElement D) {
    if (C->height * C->width != D->height * D->width) {
        printf("SE C, D should have the same size.\n");
        return NULL;
    }

    int t;
    for (t = 0; t < C->width; t++) {
        if (C->binaryImg[t] == 0 && D->binaryImg[t] == 0) {
            printf("SE C, D should not have intersection in foreground.\n");
            return NULL;
        }
    }

    int size = A->bmih.biHeight * A->bmih.biWidth;
    int i, j;

    PBMP cA = (PBMP)malloc(sizeof(BMP));
    cA->bgr = NULL;
    copyBmp(cA, A);

    for (i = 0; i < size; i++) {
        cA->bgr[i].B = 255 - A->bgr[i].B;
        cA->bgr[i].G = 255 - A->bgr[i].G;
        cA->bgr[i].R = 255 - A->bgr[i].R;
    }

    PBMP temp1 = erode(A, C);
    PBMP temp2 = erode(cA, D);

    for (i = 0; i < size; i++) {
        if (temp1->bgr[i].B == 0 && temp2->bgr[i].B == 0) {
            cA->bgr[i].B = 0;
            cA->bgr[i].G = 0;
            cA->bgr[i].R = 0;
        }
        else {
            cA->bgr[i].B = 255;
            cA->bgr[i].G = 255;
            cA->bgr[i].R = 255;
        }
    }
    
    free(temp1->bgr);
    free(temp1);
    free(temp2->bgr);
    free(temp2);

    return cA;
}