#include "geometric.h"

PBMP translate(PBMP pbmp, int offsetX, int offsetY) {
    if (offsetX < 0)
        offsetX = 0;
    if (offsetY < 0)
        offsetY = 0;

    int size = pbmp->bmih.biHeight * pbmp->bmih.biWidth;
    int newHeight = pbmp->bmih.biHeight + offsetY;
    int newWidth = pbmp->bmih.biWidth + offsetX;
    newHeight = (newHeight - 1) / 4 * 4 + 4;
    newWidth = (newWidth - 1) / 4 * 4 + 4;
    int newSize = newHeight * newWidth;

    PBMP res = (PBMP)malloc(sizeof(BMP));
    
    copyFileHeader(&res->bmfh, &pbmp->bmfh);
    copyInfoHeader(&res->bmih, &pbmp->bmih);

    res->bmfh.bfSize = 54 + newSize * 3;
    res->bmih.biHeight = newHeight;
    res->bmih.biWidth = newWidth;
    res->bmih.biSizeImage = newSize * 3;

    res->bgr = (PBGR)malloc(sizeof(BGR) * newSize);

    for (int i = 0; i < newSize; i++) {
        res->bgr[i].B = 255;
        res->bgr[i].G = 255;
        res->bgr[i].R = 255;
    }

    for (int y = 0; y < pbmp->bmih.biHeight; y++) {
        for (int x = 0; x < pbmp->bmih.biWidth; x++) {
            int newX = x + offsetX;
            int newY = y + offsetY;
            res->bgr[newY * newWidth + newX].B = pbmp->bgr[y * pbmp->bmih.biWidth + x].B;
            res->bgr[newY * newWidth + newX].G = pbmp->bgr[y * pbmp->bmih.biWidth + x].G;
            res->bgr[newY * newWidth + newX].R = pbmp->bgr[y * pbmp->bmih.biWidth + x].R;
        }
    }

    return res;
}

PBMP rotate(PBMP pbmp, double theta, int flag) {
    while (theta < 0.0f)
        theta += 360.0f;

    int diagonal = sqrt(pow(pbmp->bmih.biHeight, 2) + pow(pbmp->bmih.biWidth, 2));
    diagonal = (diagonal - 1) / 2 * 2 + 2;
    PBMP res = (PBMP)malloc(sizeof(BMP));
    
    res->bmfh.bfType = pbmp->bmfh.bfType;
    res->bmfh.bfSize = diagonal * diagonal * 4 * 3 + 54;
    res->bmfh.bfReserved1 = 0;
    res->bmfh.bfReserved2 = 0;
    res->bmfh.bfOffBits = 54;

    copyInfoHeader(&res->bmih, &pbmp->bmih);
    res->bmih.biHeight = diagonal * 2;
    res->bmih.biWidth = diagonal * 2;
    res->bmih.biSizeImage = diagonal * diagonal * 4 * 3;

    res->bgr = (PBGR)malloc(sizeof(BGR) * diagonal * diagonal * 4);
    for (int i = 0; i < diagonal * diagonal * 4; i++) {
        res->bgr[i].B = 255;
        res->bgr[i].G = 255;
        res->bgr[i].R = 255;
    }

    for (int y = 0; y < pbmp->bmih.biHeight; y++) {
        for (int x = 0; x < pbmp->bmih.biWidth; x++) {
            int newX = cos(theta) * x - sin(theta) * y;
            int newY = sin(theta) * x + cos(theta) * y;
            res->bgr[newX + diagonal + (newY + diagonal) * diagonal * 2].B = 
                pbmp->bgr[x + y * pbmp->bmih.biWidth].B;
            res->bgr[newX + diagonal + (newY + diagonal) * diagonal * 2].G = 
                pbmp->bgr[x + y * pbmp->bmih.biWidth].G;
            res->bgr[newX + diagonal + (newY + diagonal) * diagonal * 2].R = 
                pbmp->bgr[x + y * pbmp->bmih.biWidth].R;
        }
    }

    // if fill the holes
    if (flag == 1) {
        for (int y = 0; y < diagonal * 2; y++) {
            for (int x = 1; x < diagonal * 2; x++) {
                if (res->bgr[y * diagonal * 2 + x].B == 255 &&
                    res->bgr[y * diagonal * 2 + x].G == 255 &&
                    res->bgr[y * diagonal * 2 + x].R == 255 &&
                    res->bgr[y * diagonal * 2 + x + 1].B != 255 &&
                    res->bgr[y * diagonal * 2 + x + 1].G != 255 &&
                    res->bgr[y * diagonal * 2 + x + 1].R != 255) {
                    res->bgr[y * diagonal * 2 + x].B = res->bgr[y * diagonal * 2 + x - 1].B;
                    res->bgr[y * diagonal * 2 + x].G = res->bgr[y * diagonal * 2 + x - 1].G;
                    res->bgr[y * diagonal * 2 + x].R = res->bgr[y * diagonal * 2 + x - 1].R;
                }
            }
        }
    }

    return res;
}

PBMP scale(PBMP pbmp, double ratioX, double ratioY) {
    int newWidth = pbmp->bmih.biWidth * ratioX + 1;
    int newHeight = pbmp->bmih.biHeight * ratioY + 1;
    newWidth = (newWidth - 1) / 4 * 4 + 4;
    newHeight = (newHeight - 1) / 4 * 4 + 4;
    int newSize = newWidth * newHeight;

    PBMP res = (PBMP)malloc(sizeof(BMP));

    copyFileHeader(&res->bmfh, &pbmp->bmfh);
    copyInfoHeader(&res->bmih, &pbmp->bmih);

    res->bmfh.bfSize = newSize * 3 + 54;
    res->bmih.biWidth = newWidth;
    res->bmih.biHeight = newHeight;
    res->bmih.biSizeImage = newSize * 3;
    res->bgr = (PBGR)malloc(sizeof(BGR) * newSize);

    for (int i = 0; i < newSize; i++) {
        res->bgr[i].B = 255;
        res->bgr[i].G = 255;
        res->bgr[i].R = 255;
    }

    for (int y = 1; y < pbmp->bmih.biHeight; y++) {
        for (int x = 1; x < pbmp->bmih.biWidth; x++) {
            int startX = (x - 1) * ratioX;
            int endX = x * ratioX;
            int startY= (y - 1) * ratioY;
            int endY= y * ratioY;

            for (int tx = startX; tx < endX; tx++) {
                for (int ty = startY; ty < endY; ty++) {
                    res->bgr[ty * newWidth + tx].B = 
                        pbmp->bgr[y * pbmp->bmih.biWidth + x].B;
                    res->bgr[ty * newWidth + tx].G = 
                        pbmp->bgr[y * pbmp->bmih.biWidth + x].G;
                    res->bgr[ty * newWidth + tx].R = 
                        pbmp->bgr[y * pbmp->bmih.biWidth + x].R;
                }
            }
        }
    }

    return res;
}

PBMP shearOnX(PBMP pbmp, double dx) {
    int newWidth = pbmp->bmih.biWidth + dx * pbmp->bmih.biHeight;
    newWidth = (newWidth - 1) / 4 * 4 + 4;
    int newSize = pbmp->bmih.biHeight * newWidth;

    PBMP res = (PBMP)malloc(sizeof(BMP));

    copyFileHeader(&res->bmfh, &pbmp->bmfh);
    copyInfoHeader(&res->bmih, &pbmp->bmih);
    
    res->bmfh.bfSize = newSize * 3 + 54;
    res->bmih.biWidth = newWidth;
    res->bmih.biSizeImage = newSize * 3;
    
    res->bgr = (PBGR)malloc(sizeof(BGR) * newSize);

    for (int i = 0; i < newSize; i++) {
        res->bgr[i].B = 255;
        res->bgr[i].G = 255;
        res->bgr[i].R = 255;
    }

    for (int y = 0; y < pbmp->bmih.biHeight; y++) {
        for (int x = 0; x < pbmp->bmih.biWidth; x++) {
            int newX = x + dx * y;
            res->bgr[y * newWidth + newX].B = pbmp->bgr[y * pbmp->bmih.biWidth + x].B;
            res->bgr[y * newWidth + newX].G = pbmp->bgr[y * pbmp->bmih.biWidth + x].G;
            res->bgr[y * newWidth + newX].R = pbmp->bgr[y * pbmp->bmih.biWidth + x].R;
        }
    }

    return res;
}

PBMP shearOnY(PBMP pbmp, double dy) {
    int newHeight = pbmp->bmih.biHeight + (dy * pbmp->bmih.biWidth + 1);
    newHeight = (newHeight - 1) / 4 * 4 + 4;
    int newSize = newHeight * pbmp->bmih.biWidth;

    PBMP res = (PBMP)malloc(sizeof(BMP));
    
    copyFileHeader(&res->bmfh, &pbmp->bmfh);
    copyInfoHeader(&res->bmih, &pbmp->bmih);
    
    res->bmfh.bfSize = newSize * 3 + 54;
    res->bmih.biHeight = newHeight;
    res->bmih.biSizeImage = newSize * 3;
    
    res->bgr = (PBGR)malloc(sizeof(BGR) * newSize);

    for (int i = 0; i < newSize; i++) {
        res->bgr[i].B = 255;
        res->bgr[i].G = 255;
        res->bgr[i].R = 255;
    }

    for (int y = 0; y < pbmp->bmih.biHeight; y++) {
        for (int x = 0; x < pbmp->bmih.biWidth; x++) {
            int newY = dy * x + y;
            res->bgr[newY * pbmp->bmih.biWidth + x].B = pbmp->bgr[y * pbmp->bmih.biWidth + x].B;
            res->bgr[newY * pbmp->bmih.biWidth + x].G = pbmp->bgr[y * pbmp->bmih.biWidth + x].G;
            res->bgr[newY * pbmp->bmih.biWidth + x].R = pbmp->bgr[y * pbmp->bmih.biWidth + x].R;
        }
    }

    return res;
}

PBMP mirror(PBMP pbmp, int symmetryAxis) {
    int size = pbmp->bmih.biHeight * pbmp->bmih.biWidth;
    
    PBMP res = (PBMP)malloc(sizeof(BMP));

    res->bgr = (PBGR)malloc(sizeof(BGR) * size);

    copyFileHeader(&res->bmfh, &pbmp->bmfh);
    copyInfoHeader(&res->bmih, &pbmp->bmih);

    // symmetry axis is y
    if (symmetryAxis == 1) {
        for (int y = 0; y < pbmp->bmih.biHeight; y++) {
            for (int x = 0; x < pbmp->bmih.biWidth; x++) {
                int newX = pbmp->bmih.biWidth - x;
                res->bgr[y * pbmp->bmih.biWidth + newX].B = pbmp->bgr[y * pbmp->bmih.biWidth + x].B;
                res->bgr[y * pbmp->bmih.biWidth + newX].G = pbmp->bgr[y * pbmp->bmih.biWidth + x].G;
                res->bgr[y * pbmp->bmih.biWidth + newX].R = pbmp->bgr[y * pbmp->bmih.biWidth + x].R;
            }
        }
    }
    // symmetry axis is x
    else if (symmetryAxis == 2) {
        for (int y = 0; y < pbmp->bmih.biHeight; y++) {
            for (int x = 0; x < pbmp->bmih.biWidth; x++) {
                int newY = pbmp->bmih.biHeight - y;
                res->bgr[newY* pbmp->bmih.biWidth + x].B = pbmp->bgr[y * pbmp->bmih.biWidth + x].B;
                res->bgr[newY * pbmp->bmih.biWidth + x].G = pbmp->bgr[y * pbmp->bmih.biWidth + x].G;
                res->bgr[newY * pbmp->bmih.biWidth + x].R = pbmp->bgr[y * pbmp->bmih.biWidth + x].R;
            }
        }
    }
    
    return res;
}


void geometric_test() {
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

    printf("Please input the tanslation coordinates (two integers):\n");
    int offsetX, offsetY;
    scanf("%d%d", &offsetX, &offsetY);
    sprintf(save_file_path, "..\\img_output\\%s_translated_%d_%d.bmp\0", file_name, offsetX, offsetY);
    PBMP translated = translate(origin, offsetX, offsetY);
    saveBmp(save_file_path, translated);
    deleteBMP(translated);

    printf("Please input the rotation angle for hole bmp (counterclockwise, double):\n");
    double theta;
    scanf("%lf", &theta);
    sprintf(save_file_path, "..\\img_output\\%s_rotated_hole_%lf.bmp\0", file_name, theta);
    PBMP rotated_hole = rotate(origin, theta, 0);
    saveBmp(save_file_path, rotated_hole);
    deleteBMP(rotated_hole);

    printf("Please input the rotation angle for no hole bmp(counterclockwise, double):\n");
    //double theta;
    scanf("%lf", &theta);
    sprintf(save_file_path, "..\\img_output\\%s_rotated_nohole_%lf.bmp\0", file_name, theta);
    PBMP rotated_no_hole = rotate(origin, theta, 1);
    saveBmp(save_file_path, rotated_no_hole);
    deleteBMP(rotated_no_hole);

    printf("Please input the scale fcactors(two doubles):\n");
    double ratioX, ratioY;
    scanf("%lf%lf", &ratioX, &ratioY);
    sprintf(save_file_path, "..\\img_output\\%s_scaled_%lf_%lf.bmp\0", file_name, ratioX, ratioY);
    PBMP scaled = scale(origin, ratioX, ratioY);
    saveBmp(save_file_path, scaled);
    deleteBMP(scaled);

    printf("Please input the dx (a double):\n");
    double dx;
    scanf("%lf", &dx);
    sprintf(save_file_path, "..\\img_output\\%s_sheared_dx_%.3lf.bmp\0", file_name, dx);
    PBMP shearedOnX = shearOnX(origin, dx);
    saveBmp(save_file_path, shearedOnX);
    deleteBMP(shearedOnX);

    printf("Please input the dy (a double):\n");
    double dy;
    scanf("%lf", &dy);
    sprintf(save_file_path, "..\\img_output\\%s_sheared_dy_%.3lf.bmp\0", file_name, dy);
    PBMP shearedOnY = shearOnY(origin, dy);
    saveBmp(save_file_path, shearedOnY);
    deleteBMP(shearedOnY);

    sprintf(save_file_path, "..\\img_output\\%s_mirrored_y.bmp\0", file_name);
    PBMP mirroredY = mirror(origin, 1);
    saveBmp(save_file_path, mirroredY);
    deleteBMP(mirroredY);

    sprintf(save_file_path, "..\\img_output\\%s_mirrored_x.bmp\0", file_name);
    PBMP mirroredX = mirror(origin, 2);
    saveBmp(save_file_path, mirroredX);
    deleteBMP(mirroredX);
    
    printf("The output 24-bit bmp files are in the \'img_output\' folder.\n");

    deleteBMP(origin);
}