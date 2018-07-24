#include "yuv.h"

PYUVFILE bmp2yuv(PBMP pbmp)
{
    int size = pbmp->bmih.biHeight * pbmp->bmih.biWidth;
    PYUVFILE pyuvfile = (PYUVFILE)malloc(sizeof(YUVFILE));
    pyuvfile->yuv = (PYUV)malloc(sizeof(YUV) * size);

    copyFileHeader(&pyuvfile->bmfh, &pbmp->bmfh);
    copyInfoHeader(&pyuvfile->bmih, &pbmp->bmih);

    int i;
    for (i = 0; i < size; i++)
    {
        pyuvfile->yuv[i].Y = 0.299 * pbmp->bgr[i].R + 0.587 * pbmp->bgr[i].G + 0.114 * pbmp->bgr[i].B;
        pyuvfile->yuv[i].U = (-0.147) * pbmp->bgr[i].R - 0.289 * pbmp->bgr[i].G + 0.435 * pbmp->bgr[i].B;
        pyuvfile->yuv[i].V = 0.615 * pbmp->bgr[i].R - 0.515 * pbmp->bgr[i].G - 0.100 * pbmp->bgr[i].B;
    }

    return pyuvfile;
}

PBMP yuv2bmp(PYUVFILE pyuvfile)
{
    int size = pyuvfile->bmih.biHeight * pyuvfile->bmih.biWidth;
    PBMP pbmp = (PBMP)malloc(sizeof(BMP));
    pbmp->bgr = (PBGR)malloc(sizeof(BGR) * size);
    int i;

    copyFileHeader(&pbmp->bmfh, &pyuvfile->bmfh);
    copyInfoHeader(&pbmp->bmih, &pyuvfile->bmih);

    for (i = 0; i < size; i++)
    {
        double temp = 1.0000 * pyuvfile->yuv[i].Y - 0.0000 * pyuvfile->yuv[i].U + 1.1398 * pyuvfile->yuv[i].V;
        if (temp > 255.0)
            pbmp->bgr[i].R = 255;
        else
            pbmp->bgr[i].R = (unsigned char)temp;

        temp = 0.9996 * pyuvfile->yuv[i].Y - 0.3954 * pyuvfile->yuv[i].U - 0.5805 * pyuvfile->yuv[i].V;
        if (temp > 255.0)
            pbmp->bgr[i].G = 255;
        else
            pbmp->bgr[i].G = (unsigned char)temp;

        temp = 1.0020 * pyuvfile->yuv[i].Y + 2.0362 * pyuvfile->yuv[i].U - 0.0005 * pyuvfile->yuv[i].V;
        if (temp > 255.0)
            pbmp->bgr[i].B = 255;
        else
            pbmp->bgr[i].B = (unsigned char)temp;
    }

    return pbmp;
}
