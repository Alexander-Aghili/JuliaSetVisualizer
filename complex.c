#include "complex.h"

ComplexNumber* create_complex_number(double x, double y) {
    ComplexNumber* number = (ComplexNumber*) calloc(1, sizeof(ComplexNumber*));
    number->x = x;
    number->y = y;
    return number;
}
