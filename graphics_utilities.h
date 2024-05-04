#pragma once

#include "constants.h"
#include "inttypes.h"
#include "color_utilities.h"
#include <gd.h>
#include <SDL2/SDL.h>
#include <assert.h>

#define MAX_DIGITS sizeof(int) * 8 + 1
#define SUFFIX     4

void start_sdl();
void display_image(uint32_t** image_pixels);
void scene_complete();


void create_image(int f, uint32_t image_pixels[WIDTH][HEIGHT]);
