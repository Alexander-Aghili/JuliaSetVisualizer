#include "graphics_utilities.h"
#include "constants.h"

void create_image(int f, int image_pixels[WIDTH][HEIGHT]) {
    gdImagePtr im = gdImageCreate(WIDTH, HEIGHT);
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            gdImageSetPixel(im, x, y, image_pixels[x][y]);
        }
    }
    
    char file_name[MAX_DIGITS + SUFFIX];
    sprintf(file_name, "%d.jpg", f);

    FILE* jpeg = fopen(file_name, "wb");
    gdImageJpeg(im, jpeg, -1);
    fclose(jpeg);

    gdImageDestroy(im);

}
