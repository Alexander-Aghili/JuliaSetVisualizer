#pragma once
#include "constants.h"
#include "stdlib.h"
#include <inttypes.h>

typedef struct {
    double max_img;
    double max_real;
    double min_img;
    double min_real;
} ComplexBounds;

typedef struct {
    double x; // real
    double y; // imaginary
} ComplexNumber;

typedef struct {
    ComplexNumber *c;
    ComplexBounds *bounds;
} ComplexScene;


//ComplexNumber* create_complex_number(double x, double y);
