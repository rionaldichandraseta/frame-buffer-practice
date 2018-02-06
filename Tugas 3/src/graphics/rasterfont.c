#include "rasterfont.h"

void initRasterFont(RasterFont *rasterFont) {
    rasterFont->dict = (Shape *)malloc(256 * sizeof(Shape));
    rasterFont->height = 90;
    rasterFont->width = 110;
}

void openRasterFont(char *rasterFontChar, RasterFont *rasterFont) {
    for (int i = 'a'; i <= 'z'; i++) {
        initShape(&(rasterFont->dict[i]), 10);
        for (int j = 0; j < 10; j++) {
            initPolygon(&(rasterFont->dict[i].polygons[j]), 20);
        }
    }

    char rasterFontFilename[30];
    strcpy(rasterFontFilename, "fonts/");
    strcat(rasterFontFilename, rasterFontChar);
    strcat(rasterFontFilename, ".txt");

    char charDump[2];
    FILE *rasterFontFile;
    rasterFontFile = fopen(rasterFontFilename, "r");
    if (rasterFontFile) {
        fscanf(rasterFontFile, "%d", &(rasterFont->width));
        fscanf(rasterFontFile, "%d", &(rasterFont->height));

        int x, y;
        while (fscanf(rasterFontFile, "%s", charDump) == 1) {
            // Let the party begin
            // Check for -1,-1 which means new polygon
            // Check for -9,-9 which means new char
            // Check for -999,-999 which means end of file

            int currentChar = charDump[0];

            int polygonIndex = 0;
            int verticeIndex = 0;
            while (fscanf(rasterFontFile, "%d,%d", &x, &y) == 2) {
                if ((x == -999 && y == -999) || (x == -9 && y == -9)) {
                    rasterFont->dict[currentChar]
                        .polygons[polygonIndex]
                        .vertexCount = verticeIndex;
                    rasterFont->dict[currentChar].polygonCount =
                        polygonIndex + 1;

                    break;
                } else if (x == -1 && y == -1) {
                    rasterFont->dict[currentChar]
                        .polygons[polygonIndex]
                        .vertexCount = verticeIndex;

                    polygonIndex++;
                    verticeIndex = 0;
                } else {
                    rasterFont->dict[currentChar]
                        .polygons[polygonIndex]
                        .vertices[verticeIndex]
                        .x = x;
                    rasterFont->dict[currentChar]
                        .polygons[polygonIndex]
                        .vertices[verticeIndex]
                        .y = y;

                    verticeIndex++;
                }
            }
        }
    }

    fclose(rasterFontFile);
}
