#include <time.h>

#include "filter.h"

double distance2D(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double distance3D(int x1, int y1, int z1, int x2, int y2, int z2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
}

double RGBDistanceSquare3D(BGR bgr1, BGR bgr2) {
    return pow(bgr1.B - bgr2.B, 2) + pow(bgr1.G - bgr2.G, 2) + pow(bgr1.R - bgr2.R, 2);
}

double gaussian(int x, int sigma) {
    return exp(-(pow(x, 2)) / (2 * pow(sigma, 2))) / (sigma * SQRT_2PI);
}

PBMP meanFilter(PBMP pbmp) {
    int h = pbmp->bmih.biHeight;
    int w = pbmp->bmih.biWidth;
    int size = h * w;

    PBMP res = (PBMP)malloc(sizeof(BMP));

    copyFileHeader(&res->bmfh, &pbmp->bmfh);
    copyInfoHeader(&res->bmih, &pbmp->bmih);

    res->bgr = (PBGR)malloc(sizeof(BGR) * size);

    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            res->bgr[y * w + x].B = (
                pbmp->bgr[(y - 1) * w + x - 1].B + pbmp->bgr[(y - 1) * w + x].B + pbmp->bgr[(y - 1) * w + x + 1].B +
                pbmp->bgr[y * w + x - 1].B + pbmp->bgr[y * w + x].B + pbmp->bgr[y * w + x + 1].B +
                pbmp->bgr[(y + 1) * w + x - 1].B + pbmp->bgr[(y + 1) * w + x].B + pbmp->bgr[(y + 1) * w + x + 1].B
            ) / 9;
            res->bgr[y * w + x].G = (
                pbmp->bgr[(y - 1) * w + x - 1].G + pbmp->bgr[(y - 1) * w + x].G + pbmp->bgr[(y - 1) * w + x + 1].G +
                pbmp->bgr[y * w + x - 1].G + pbmp->bgr[y * w + x].G + pbmp->bgr[y * w + x + 1].G +
                pbmp->bgr[(y + 1) * w + x - 1].G + pbmp->bgr[(y + 1) * w + x].G + pbmp->bgr[(y + 1) * w + x + 1].G
            ) / 9;
            res->bgr[y * w + x].R = (
                pbmp->bgr[(y - 1) * w + x - 1].R + pbmp->bgr[(y - 1) * w + x].R + pbmp->bgr[(y - 1) * w + x + 1].R +
                pbmp->bgr[y * w + x - 1].R + pbmp->bgr[y * w + x].R + pbmp->bgr[y * w + x + 1].R +
                pbmp->bgr[(y + 1) * w + x - 1].R + pbmp->bgr[(y + 1) * w + x].R + pbmp->bgr[(y + 1) * w + x + 1].R
            ) / 9;
        }
    }

    // edges
    for (int y = 1; y < h - 1; y++) {
        res->bgr[y * w + 0].B = (
            pbmp->bgr[(y - 1) * w + 0].B + pbmp->bgr[(y - 1) * w + 1].B + 
            pbmp->bgr[y * w + 0].B + pbmp->bgr[y * w + 1].B + 
            pbmp->bgr[(y + 1) * w + 0].B + pbmp->bgr[(y + 1) * w + 1].B
        ) / 6;
        
        res->bgr[y * w + w - 1].B = (
            pbmp->bgr[(y - 1) * w + w - 2].B + pbmp->bgr[(y - 1) * w + w - 1].B + 
            pbmp->bgr[y * w + w - 2].B + pbmp->bgr[y * w + w - 1].B + 
            pbmp->bgr[(y + 1) * w + w - 2].B + pbmp->bgr[(y + 1) * w + w - 1].B
        ) / 6;
    }
    for (int x = 1; x < w - 1; x++) {
        res->bgr[0 * w + x].B = (
            pbmp->bgr[0 * w + x - 1].B + pbmp->bgr[1 * w + x - 1].B + 
            pbmp->bgr[0 * w + x].B + pbmp->bgr[1 * w + x].B + 
            pbmp->bgr[0 * w + x + 1].B + pbmp->bgr[1 * w + x + 1].B
        ) / 6;

        res->bgr[(h - 1) * w + x].B = (
            pbmp->bgr[(h - 1) * w + x - 1].B + pbmp->bgr[(h - 2) * w + x - 1].B + 
            pbmp->bgr[(h - 1) * w + x].B + pbmp->bgr[(h - 2) * w + x].B + 
            pbmp->bgr[(h - 1) * w + x + 1].B + pbmp->bgr[(h - 2) * w + x + 1].B
        ) / 6;
    }

    // corners
    res->bgr[0 * w + 0].B = (
        pbmp->bgr[0 * w + 0].B + pbmp->bgr[0 * w + 1].B + 
        pbmp->bgr[1 * w + 0].B + pbmp->bgr[1 * w + 1].B
    ) / 4;
    res->bgr[0 * w + 0].G = (
        pbmp->bgr[0 * w + 0].G + pbmp->bgr[0 * w + 1].G + 
        pbmp->bgr[1 * w + 0].G + pbmp->bgr[1 * w + 1].G
    ) / 4;
    res->bgr[0 * w + 0].R = (
        pbmp->bgr[0 * w + 0].R + pbmp->bgr[0 * w + 1].R + 
        pbmp->bgr[1 * w + 0].R + pbmp->bgr[1 * w + 1].R
    ) / 4;
    
    res->bgr[0 * w + w - 1].B = (
        pbmp->bgr[0 * w + w - 2].B + pbmp->bgr[0 * w + w - 1].B +
        pbmp->bgr[1 * w + w - 2].B + pbmp->bgr[1 * w + w - 1].B
    ) / 4;
    res->bgr[0 * w + w - 1].G = (
        pbmp->bgr[0 * w + w - 2].G + pbmp->bgr[0 * w + w - 1].G +
        pbmp->bgr[1 * w + w - 2].G + pbmp->bgr[1 * w + w - 1].G
    ) / 4;
    res->bgr[0 * w + w - 1].R = (
        pbmp->bgr[0 * w + w - 2].R + pbmp->bgr[0 * w + w - 1].R + 
        pbmp->bgr[1 * w + w - 2].R + pbmp->bgr[1 * w + w - 1].R
    ) / 4;

    res->bgr[(h - 1) * w + 0].B = (
        pbmp->bgr[(h - 1) * w + 0].B + pbmp->bgr[(h - 1) * w + 1].B +
        pbmp->bgr[(h - 2) * w + 0].B + pbmp->bgr[(h - 2) * w + 1].B
    ) / 4;
    res->bgr[(h - 1) * w + 0].G = (
        pbmp->bgr[(h - 1) * w + 0].G + pbmp->bgr[(h - 1) * w + 1].G + 
        pbmp->bgr[(h - 2) * w + 0].G + pbmp->bgr[(h - 2) * w + 1].G
    ) / 4;
    res->bgr[(h - 1) * w + 0].R = (
        pbmp->bgr[(h - 1) * w + 0].R + pbmp->bgr[(h - 1) * w + 1].R + 
        pbmp->bgr[(h - 2) * w + 0].R + pbmp->bgr[(h - 2) * w + 1].R
    ) / 4;

    res->bgr[(h - 1) * w + w - 1].B = (
        pbmp->bgr[(h - 2) * w + w - 2].B + pbmp->bgr[(h - 2) * w + w - 1].B + 
        pbmp->bgr[(h - 1) * w + w - 2].B + pbmp->bgr[(h - 1) * w + w - 1].B
    ) / 4;
    res->bgr[(h - 1) * w + w - 1].G = (
        pbmp->bgr[(h - 2) * w + w - 2].G + pbmp->bgr[(h - 2) * w + w - 1].G + 
        pbmp->bgr[(h - 1) * w + w - 2].G + pbmp->bgr[(h - 1) * w + w - 1].G
    ) / 4;
    res->bgr[(h - 1) * w + w - 1].R = (
        pbmp->bgr[(h - 2) * w + w - 2].R + pbmp->bgr[(h - 2) * w + w - 1].R + 
        pbmp->bgr[(h - 1) * w + w - 2].R + pbmp->bgr[(h - 1) * w + w - 1].R
    ) / 4;


    return res;
}

PBMP laplacianEnhance(PBMP pbmp) {
    int h = pbmp->bmih.biHeight;
    int w = pbmp->bmih.biWidth;
    int size = h * w;

    PBMP res = (PBMP)malloc(sizeof(BMP));

    copyFileHeader(&res->bmfh, &pbmp->bmfh);
    copyInfoHeader(&res->bmih, &pbmp->bmih);

    res->bgr = (PBGR)malloc(sizeof(BGR) * size);

    IntBGR **laplacianRes = (IntBGR**)malloc(sizeof(IntBGR*) * h);
    for (int y = 0; y < h; y++)
        laplacianRes[y] = (IntBGR*)malloc(sizeof(IntBGR) * w);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            laplacianRes[y][x].B = 0;
            laplacianRes[y][x].G = 0;
            laplacianRes[y][x].R = 0;
        }
    }

    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            for (int ty = y - 1; ty <= y + 1; ty++) {
                for (int tx = x - 1; tx <= x + 1; tx++) {
                    laplacianRes[y][x].B += pbmp->bgr[ty * w + tx].B;
                    laplacianRes[y][x].G += pbmp->bgr[ty * w + tx].G;
                    laplacianRes[y][x].R += pbmp->bgr[ty * w + tx].R;
                }
            }
            
            laplacianRes[y][x].B -= 9 * pbmp->bgr[y * w + x].B;
            laplacianRes[y][x].G -= 9 * pbmp->bgr[y * w + x].G;
            laplacianRes[y][x].R -= 9 * pbmp->bgr[y * w + x].R;
        }
    }
    
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            laplacianRes[y][x].B = pbmp->bgr[y * w + x].B - laplacianRes[y][x].B;
            if (laplacianRes[y][x].B > 255)
                res->bgr[y * w + x].B = 255;
            else if (laplacianRes[y][x].B < 0)
                res->bgr[y * w + x].B = 0;
            else
                res->bgr[y * w + x].B = laplacianRes[y][x].B;

            laplacianRes[y][x].G = pbmp->bgr[y * w + x].G - laplacianRes[y][x].G;
            if (laplacianRes[y][x].G > 255)
                res->bgr[y * w + x].G = 255;
            else if (laplacianRes[y][x].G < 0)
                res->bgr[y * w + x].G = 0;
            else
                res->bgr[y * w + x].G = laplacianRes[y][x].G;

            laplacianRes[y][x].R = pbmp->bgr[y * w + x].R - laplacianRes[y][x].R;
            if (laplacianRes[y][x].R > 255)
                res->bgr[y * w + x].R = 255;
            else if (laplacianRes[y][x].R < 0)
                res->bgr[y * w + x].R = 0;
            else
                res->bgr[y * w + x].R = laplacianRes[y][x].R;
        }
    }

    for (int y = 0; y < h; y++) 
        free(laplacianRes[y]);
    free(laplacianRes);

    return res;
}

PBMP bilateralFilter(PBMP pbmp, int winR, int sigmaS, int sigmaR) {
    int w = pbmp->bmih.biWidth;
    int h = pbmp->bmih.biHeight;
    int size = w * h;

    PBMP res = (PBMP)malloc(sizeof(BMP));
    
    copyFileHeader(&res->bmfh, &pbmp->bmfh);
    copyInfoHeader(&res->bmih, &pbmp->bmih);

    res->bgr = (PBGR)malloc(sizeof(BGR) * size);
    
    // deal with the main part, ignore the edges and corners
    for (int y = sigmaS; y < h - sigmaS; y++) {
        for (int x = sigmaS; x < w - sigmaS; x++) {
            // For each channel, do the same operation

            // Wp is for normalization, so it is the sum of factor for each pixel in the mask
            double Wp = 0.0f;
            // bfValueB = G_sigmaS(|p - q|) * G_sigmaR(|I_p - I_q|) * I_q
            double bfValueB = 0.0f;
            double bfValueG = 0.0f;
            double bfValueR = 0.0f;
            for (int ty = y - sigmaS; ty <= y + sigmaS; ty++) {
                for (int tx = x - sigmaS; tx <= x + sigmaS; tx++) {
                    // factor 1 is the space part
                    double factor1 = gaussian(distance2D(tx, ty, x, y), sigmaS);
                    // factor 2 is the color part
                    double factor2 = gaussian(RGBDistanceSquare3D(pbmp->bgr[y * w + x], pbmp->bgr[ty * w + x]), sigmaR);
                    // calculate the term of sum
                    bfValueB += factor1 * factor2 * pbmp->bgr[ty * w + tx].B;
                    bfValueG += factor1 * factor2 * pbmp->bgr[ty * w + tx].G;
                    bfValueR += factor1 * factor2 * pbmp->bgr[ty * w + tx].R;
                    // calculate Wp
                    Wp += factor1 * factor2;
                }
            }
            // divide by Wp
            bfValueB /= Wp;
            bfValueG /= Wp;
            bfValueR /= Wp;

            res->bgr[y * w + x].B = bfValueB;
            res->bgr[y * w + x].G = bfValueG;
            res->bgr[y * w + x].R = bfValueR;
        }
    }

    // for border area, keep original values
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (x < sigmaS || y < sigmaS || x >= w - sigmaS || y >= h - sigmaS) {
                res->bgr[y * w + x].B = pbmp->bgr[y * w + x].B;
                res->bgr[y * w + x].G = pbmp->bgr[y * w + x].G;
                res->bgr[y * w + x].R = pbmp->bgr[y * w + x].R;
            } 
        }
    }

    return res;
}


PBMP bilateralFilterOpt(PBMP pbmp, int winR, int sigmaS, int sigmaR) {
    int w = pbmp->bmih.biWidth;
    int h = pbmp->bmih.biHeight;
    int size = w * h;

    PBMP res = (PBMP)malloc(sizeof(BMP));
    
    copyFileHeader(&res->bmfh, &pbmp->bmfh);
    copyInfoHeader(&res->bmih, &pbmp->bmih);

    res->bgr = (PBGR)malloc(sizeof(BGR) * size);

    // pre-calculate the unchanged part of gaussian function
    double gaussian_s_pre = 1.0 / (sigmaS * sqrt(2 * PI));
    double gaussian_r_pre = 1.0 / (sigmaR * sqrt(2 * PI));
    double gaussian_s_coe = -0.5 / (sigmaS * sigmaS);
    double gaussian_r_coe = -0.5 / (sigmaR * sigmaR);

    // pre-calculate a weight table for the mask
    double **spaceWeightTable = (double**)malloc(sizeof(double*) * (winR * 2 + 1));
    for (int i = 0; i < (winR * 2 + 1); i++)
        spaceWeightTable[i] = (double *)malloc(sizeof(double) * (winR * 2 + 1));
    for (int i = -winR; i <= winR; i++) {
        for (int j = -winR; j <= winR; j++) {
            int y = i + winR;
            int x = j + winR;
            spaceWeightTable[y][x] = gaussian_s_pre *  exp(gaussian_s_coe * (i * i + j * j));
        }
    }

    /*
    // only for gray image
    // pre-calculate a intensity weight table for the absolute value of difference of pixel values
    double *intensityWeightTable = (double *)malloc(sizeof(double) * 256);
    for (int i = 0; i < 256; i++) {
        intensityWeightTable[i] = gaussian_r_pre * exp(gaussian_r_coe * i * i);
    }
    */

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            double weightSum = 0.0f;

            double pixelSumB = 0.0f;
            double pixelSumG = 0.0f;
            double pixelSumR = 0.0f;
            for (int i = -winR; i <= winR; i++) {
                for (int j = -winR; j <= winR; j++) {
                    if (i * i + j * j > winR * winR)
                        continue;

                    // deal with the border
                    int xTmp = x + j;
                    int yTmp = y + i;
                    xTmp = xTmp < 0 ? 0 : xTmp;
                    xTmp = xTmp > w - 1 ? w - 1 : xTmp;
                    yTmp = yTmp < 0 ? 0 : yTmp;
                    yTmp = yTmp > h - 1 ? h - 1 : yTmp;

                    int pixelDif = abs(pbmp->bgr[yTmp * w + xTmp].B - pbmp->bgr[y * w + x].B);
                    // the calculation of the factor 1 is replaced by table lookup
                    double colorWeight = gaussian_r_pre * exp(gaussian_r_coe * 
                        RGBDistanceSquare3D(pbmp->bgr[yTmp * w + xTmp], pbmp->bgr[y * w + x]));
                    double weightTmp = spaceWeightTable[i + winR][j + winR] * colorWeight;

                    pixelSumB += pbmp->bgr[yTmp * w + xTmp].B * weightTmp;
                    pixelSumG += pbmp->bgr[yTmp * w + xTmp].G * weightTmp;
                    pixelSumR += pbmp->bgr[yTmp * w + xTmp].R * weightTmp;
                    weightSum += weightTmp;
                }
            }
            pixelSumB /= weightSum;
            pixelSumB = pixelSumB < 0 ? 0 : pixelSumB;
            pixelSumB = pixelSumB > 255 ? 255 : pixelSumB;

            pixelSumG /= weightSum;
            pixelSumG = pixelSumG < 0 ? 0 : pixelSumG;
            pixelSumG = pixelSumG > 255 ? 255 : pixelSumG;

            pixelSumR /= weightSum;
            pixelSumR = pixelSumR < 0 ? 0 : pixelSumR;
            pixelSumR = pixelSumR > 255 ? 255 : pixelSumR;

            res->bgr[y * w + x].B = pixelSumB;
            res->bgr[y * w + x].G = pixelSumG;
            res->bgr[y * w + x].R = pixelSumR;
        }
    }
    
    return res;
}

void filterTestForAssignment5() {
    PBMP origin = (PBMP)malloc(sizeof(BMP));
    origin->bgr = NULL;

    char file_path[100] = { 0 };
    char file_name[100] = { 0 };
    char save_file_path[100] = { 0 };

    printf("Please input the path of the origin 24-bit bmp file.\n");
    printf("For test, you could input \'messi.bmp\'\n");
    scanf("%s", file_path);
    int foundSlash = 0;
    for (int i = strlen((char *)file_path); i >= 0; i--) {
        if (file_path[i] == '/' || file_path[i] == '\\') {
            sprintf(file_name, "%s\0", file_path + i + 1);
            for (int j = strlen((char *)file_name); j >= 0; j--) {
                if (file_name[j] == '.') {
                    for (int k = j; k < sizeof(file_name); k++)
                        file_name[k] = 0;
                    break;
                }
            }
            foundSlash = 1;
            break;
        }
    }
    if (!foundSlash) {
        sprintf(file_name, "%s\0", file_path);
        for (int j = strlen((char *)file_name); j >= 0; j--) {
            if (file_name[j] == '.') {
                for (int k = j; k < sizeof(file_name); k++)
                    file_name[k] = 0;
                break;
            }
        }
    }

    loadBmp(file_path, origin);

    sprintf(save_file_path, "..\\img_output\\%s_mean_filtered.bmp\0", file_name);
    PBMP meanFiltered = meanFilter(origin);
    saveBmp(save_file_path, meanFiltered);
    deleteBMP(meanFiltered);

    sprintf(save_file_path, "..\\img_output\\%s_laplacian_enhanced.bmp\0", file_name);
    PBMP laplacianEnhanced = laplacianEnhance(origin);
    saveBmp(save_file_path, laplacianEnhanced);
    deleteBMP(laplacianEnhanced);

   deleteBMP(origin);
}

void filterTestForAssignment6() {
    PBMP origin = (PBMP)malloc(sizeof(BMP));
    origin->bgr = NULL;

    char file_path[100] = { 0 };
    char file_name[100] = { 0 };
    char save_file_path[100] = { 0 };

    printf("Please input the path of the origin 24-bit bmp file.\n");
    printf("For test, you could input \'c.bmp\'\n");
    scanf("%s", file_path);
    int foundSlash = 0;
    for (int i = strlen((char *)file_path); i >= 0; i--) {
        if (file_path[i] == '/' || file_path[i] == '\\') {
            sprintf(file_name, "%s\0", file_path + i + 1);
            for (int j = strlen((char *)file_name); j >= 0; j--) {
                if (file_name[j] == '.') {
                    for (int k = j; k < sizeof(file_name); k++)
                        file_name[k] = 0;
                    break;
                }
            }
            foundSlash = 1;
            break;
        }
    }
    if (!foundSlash) {
        sprintf(file_name, "%s\0", file_path);
        for (int j = strlen((char *)file_name); j >= 0; j--) {
            if (file_name[j] == '.') {
                for (int k = j; k < sizeof(file_name); k++)
                    file_name[k] = 0;
                break;
            }
        }
    }

    loadBmp(file_path, origin);

    clock_t start, end;

    int winR, sigmaS, sigmaR;
    printf("Please input the parameters of bilateral filter (winR, sigmaS, sigmaR).\n");
    scanf("%d%d%d", &winR, &sigmaS, &sigmaR);

    // optimization version
    sprintf(save_file_path, "..\\img_output\\%s_bilateral_filtered_opt_%d_%d_%d.bmp\0", file_name, winR, sigmaS, sigmaR);
    start = clock();
    PBMP bilateralFilteredOpt = bilateralFilterOpt(origin, winR, sigmaS, sigmaR);
    end = clock();
    printf("parameters: winR = %d, s = %d, r = %d, opt time cost: %d sec\n", winR, sigmaS, sigmaR, (end - start) / CLOCKS_PER_SEC);
    saveBmp(save_file_path, bilateralFilteredOpt);
    deleteBMP(bilateralFilteredOpt);

    /*
    // windows radius, area = (winR * 2 + 1) ^ 2
    for (int winR = 1; winR < 16; winR *= 4) {
        // sigma S
        for (int s = 1; s <= 16; s *= 2) {
            // sigma R
            for (int r = 1; r <= 1024; r *= 2) {
                // no optimization version
                sprintf(save_file_path, "img_output\\%s_bilateral_filtered_%d_%d_%df.bmp\0", file_name, winR, s, r);
                start = clock();
                PBMP bilateralFiltered = bilateralFilter(origin, winR, s, r);
                end = clock();
                printf("parameters: winR = %d, s = %d, r = %d, no opt time cost: %d sec\n", winR, s, r, (end - start) / CLOCKS_PER_SEC);
                saveBmp(save_file_path, bilateralFiltered);
                deleteBMP(bilateralFiltered);

                // optimization version
                sprintf(save_file_path, "img_output\\%s_bilateral_filtered_opt_%d_%d_%d.bmp\0", file_name, s, r);
                start = clock();
                PBMP bilateralFilteredOpt = bilateralFilterOpt(origin, winR, s, r);
                end = clock();
                printf("parameters: winR = %d, s = %d, r = %d, opt time cost: %d sec\n", winR, s, r, (end - start) / CLOCKS_PER_SEC);
                saveBmp(save_file_path, bilateralFilteredOpt);
                deleteBMP(bilateralFilteredOpt);
            }
        }
    }
    */
    
    deleteBMP(origin);

    printf("The output is in the \'img_output\' folder.\n");
}