#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "constants.h"
#include "math.h"

#define NUM_COLORS 16
static uint32_t color_map[NUM_COLORS];
static uint32_t black;

uint32_t get_int_from_color(int red, int green, int blue);
void initialize_color_map();
uint32_t get_color(int n);
int color_point(double a, double b);
