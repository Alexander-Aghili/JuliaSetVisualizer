#include "color_utilities.h"

uint32_t get_int_from_color(int red, int green, int blue) {
    red = (red << 16) & 0x00FF0000;
    green = (green << 8) & 0x0000FF00;
    blue = blue & 0x000000FF;

    return 0x00000000 | red | green | blue;
}

void initialize_color_map() {
    color_map[0] = get_int_from_color(66, 30, 15);
    color_map[1] = get_int_from_color(25, 7, 26);
    color_map[2] = get_int_from_color(9, 1, 47);
    color_map[3] = get_int_from_color(4, 4, 73);
    color_map[4] = get_int_from_color(0, 7, 100);
    color_map[5] = get_int_from_color(12, 44, 138);
    color_map[6] = get_int_from_color(24, 82, 177);
    color_map[7] = get_int_from_color(57, 135, 209);
    color_map[8] = get_int_from_color(134, 181, 229);
    color_map[9] = get_int_from_color(211, 236, 248);
    color_map[10] = get_int_from_color(241, 233, 191);
    color_map[11] = get_int_from_color(248, 201, 95);
    color_map[12] = get_int_from_color(255, 170, 0);
    color_map[13] = get_int_from_color(204, 128, 0);
    color_map[14] = get_int_from_color(153, 87, 0);
    color_map[15] = get_int_from_color(106, 52, 3);

    black = get_int_from_color(0, 0, 0);
}

/**
     * Taken from https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia
     * 
     */
uint32_t get_color(int n) {
    if (n < MAX_ITERATIONS && n > 0) {
        int i = n % 16;
        return color_map[i];
    }
    return black;
}

int color_point(double a, double b) {
    double x = a;
    double y = b;

    int n = 0;
    while (n < MAX_ITERATIONS) { 
        double u = a*a - b*b;
        double v = 2*a*b;

        if (fabs(u+v) > THRESHOLD) {
            break;
        }

        a = u + x;
        b = v + y;

        n++;
    }

    return n;
}
