#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "math.h"

#define NUM_COLORS 16
static int color_map[NUM_COLORS];
static int black;

int get_int_from_color(int red, int green, int blue);
void initialize_color_map();
int get_color(int n);
int color_point(double a, double b);
