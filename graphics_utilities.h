#pragma once

#include "constants.h"
#include <gd.h>

#define MAX_DIGITS sizeof(int)*8 + 1
#define SUFFIX 4

void create_image(int f, int image_pixels[WIDTH][HEIGHT]);
