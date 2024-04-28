#include "graphics_utilities.h"
#include "constants.h"

void create_image(int f, uint32_t image_pixels[WIDTH][HEIGHT]) {
    gdImagePtr im = gdImageCreateTrueColor(WIDTH, HEIGHT);
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            gdImageSetPixel(im, x, y, image_pixels[x][y]);
        }
    }

    char file_name[MAX_DIGITS + SUFFIX];
    sprintf(file_name, "%d.png", f);

    FILE *img = fopen(file_name, "wb");
    gdImagePng(im, img);
    fclose(img);

    gdImageDestroy(im);
}
