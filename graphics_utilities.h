#pragma once

#include "constants.h"
#include "inttypes.h"
#include <gd.h>

#define MAX_DIGITS sizeof(int) * 8 + 1
#define SUFFIX     4

void create_image(int f, uint32_t image_pixels[WIDTH][HEIGHT]);
