#pragma once
#include <stdint.h>
#include "constants.h"
#include "complex.h"

#define THREADS_PER_BLOCK 16

void add_pixel(ComplexBounds* scene_bounds, ComplexNumber* c, uint32_t* image_pixels);
uint32_t* create_image_pixels_arr(int x, int y);
