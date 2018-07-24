#include "bmp.h"


int loadBmp(const char *file_path, PBMP pbmp) {
    FILE *fp = fopen(file_path, "rb");

    if (fp == NULL) {
        printf("open file failed.\n");
        return 1;
    }

    int size;
    int i;

    if (pbmp == NULL) {
        pbmp = (PBMP)malloc(sizeof(BMP));
        if (pbmp == NULL) {
            printf("no enough space. allocate space for bmp failed.\n");
            return -1;
        }
        pbmp->bgr = NULL;
    }

    fread(&pbmp->bmfh, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&pbmp->bmih, sizeof(BITMAPINFOHEADER), 1, fp);

    if (pbmp->bmih.biBitCount != 24) {
        printf("the file is not 24-bit bitmap!\n");
        return -2;
    }

    
    if (pbmp->bgr != NULL)
        free(pbmp->bgr);

    size = pbmp->bmih.biWidth * pbmp->bmih.biHeight;
    pbmp->bgr = (PBGR)malloc(sizeof(BGR) * size);
    if (pbmp->bgr == NULL) {
        printf("no enought space, allocate space for bgr failed.\n");
        return -3;
    }

    for (i = 0; i < size; i++)
        fread(pbmp->bgr + i, sizeof(BGR), 1, fp);

    fclose(fp);

    return 0;
}

int saveBmp(const char *file_path, BMP *pbmp) {
    if (pbmp == NULL) {
        printf("empty bmp, save bmp failed.\n");
        return -1;
    }

    FILE *fp = fopen(file_path, "wb");
    int size = pbmp->bmih.biWidth * pbmp->bmih.biHeight;
    int i;

    fwrite(&pbmp->bmfh, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&pbmp->bmih, sizeof(BITMAPINFOHEADER), 1, fp);

    for (i = 0; i < size; i++)
        fwrite(pbmp->bgr + i, sizeof(BGR), 1, fp);

    fclose(fp);

    return 0;
}

int copyBmp(PBMP des, PBMP src) {
    if (des == NULL)
        return -1;
    if (src == NULL)
        return -2;
    copyFileHeader(&des->bmfh, &src->bmfh);
    copyInfoHeader(&des->bmih, &src->bmih);
    int size = src->bmih.biHeight * src->bmih.biWidth;
    if (des->bgr != NULL)
        free(des->bgr);
    des->bgr = (PBGR)malloc(sizeof(BGR) * size);
    int i;
    for (i = 0; i < size; i++) {
        des->bgr[i].B = src->bgr[i].B;
        des->bgr[i].G = src->bgr[i].G;
        des->bgr[i].R = src->bgr[i].R;
    }
    return 0;
}

int copyFileHeader(BITMAPFILEHEADER *destination, BITMAPFILEHEADER *source) {
    if (destination == NULL)
        return -1;
    if (source == NULL)
        return -2;

    destination->bfType = source->bfType;
    destination->bfOffBits = source->bfOffBits;
    destination->bfSize = source->bfSize;
    destination->bfReserved1 = source->bfReserved1;
    destination->bfReserved2 = source->bfReserved2;

    return 0;
}

int copyInfoHeader(BITMAPINFOHEADER *destination, BITMAPINFOHEADER *source) {
    if (destination == NULL)
        return -1;
    if (source == NULL)
        return -2;

    destination->biBitCount = source->biBitCount;
    destination->biClrImportant = source->biClrImportant;
    destination->biClrUsed = source->biClrUsed;
    destination->biCompression = source->biCompression;
    destination->biHeight = source->biHeight;
    destination->biWidth = source->biWidth;
    destination->biSize = source->biSize;
    destination->biSizeImage = source->biSizeImage;
    destination->biPlanes = source->biPlanes;
    destination->biXPelsPerMeter = source->biXPelsPerMeter;
    destination->biYPelsPerMeter = source->biYPelsPerMeter;

    return 0;
}

int printHeader(PBMP pbmp) {
    if (pbmp == NULL) {
        printf("null bmp. fail to print header.\n");
        return -1;
    }

    printf("File header\n");
    printf("bfType: %d\n", pbmp->bmfh.bfType);
    printf("bfSize: %d\n", pbmp->bmfh.bfSize);
    printf("biReserved1: %d\n", pbmp->bmfh.bfReserved1);
    printf("biReserved2: %d\n", pbmp->bmfh.bfReserved2);
    printf("biOffBits: %d\n", pbmp->bmfh.bfOffBits);

    printf("\nInfo header\n");
    printf("biSize: %x\n", pbmp->bmih.biSize);
    printf("biWidth: %x\n", pbmp->bmih.biWidth);
    printf("biHeight: %x\n", pbmp->bmih.biHeight);
    printf("biPlanes: %x\n", pbmp->bmih.biPlanes);
    printf("biBitCount: %x\n", pbmp->bmih.biBitCount);
    printf("biCompression: %x\n", pbmp->bmih.biCompression);
    printf("biSizeImage: %x\n", pbmp->bmih.biSizeImage);
    printf("biXPelsPerMeter: %x\n", pbmp->bmih.biXPelsPerMeter);
    printf("biYPelsPerMeter: %x\n", pbmp->bmih.biYPelsPerMeter);
    printf("biClrUsed: %x\n", pbmp->bmih.biClrUsed);
    printf("biClrImportant: %x\n", pbmp->bmih.biClrImportant);

    return 0;
}

void deleteBMP(PBMP pbmp) {
    free(pbmp->bgr);
    free(pbmp);
}

PBMP garyscale(PBMP A) {
    int size = A->bmih.biHeight * A->bmih.biWidth;
    PBMP res = (PBMP)malloc(sizeof(BMP));
    res->bgr = (PBGR)malloc(sizeof(BGR) * size);
    copyFileHeader(&res->bmfh, &A->bmfh);
    copyInfoHeader(&res->bmih, &A->bmih);

    int i;
    for (i = 0; i < size; i++)
    {
        unsigned char temp = (unsigned char)(0.299 * A->bgr[i].R + 0.587 * A->bgr[i].G + 0.114 * A->bgr[i].B);
        res->bgr[i].B = res->bgr[i].G = res->bgr[i].R = temp;
    }

    return res;
}