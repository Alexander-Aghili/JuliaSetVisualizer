#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "constants.h"
#include "color_utilities.h"
#include "complex.h"
#include "assert.h"

ComplexScene* create_complex_scene(ComplexNumber* c, ComplexBounds* start, ComplexBounds* end) ;

int main(int argc, char** argv) {
    ComplexScene* scene = create_complex_scene(NULL, NULL, NULL);
}

ComplexScene* create_complex_scene(ComplexNumber* c, ComplexBounds* start, ComplexBounds* end) {
    ComplexScene* scene = (ComplexScene*) calloc(1, sizeof(ComplexScene));
    assert(scene != NULL);
    scene->c = c;

    if (start == NULL) {
        start = calloc(1, sizeof(ComplexBounds));
        assert(start != NULL);
        start->max_img = DEFAULT_START_MAX_IMG;
        start->min_img = DEFAULT_START_MIN_IMG;
        start->max_real = DEFAULT_START_MAX_REAL;
        start->min_real = DEFAULT_START_MIN_REAL;
    }

    if (end == NULL) {
        end = calloc(1, sizeof(ComplexBounds));
        assert(end != NULL);
        end->max_img = DEFAULT_END_MAX_IMG;
        end->min_img = DEFAULT_END_MIN_IMG;
        end->max_real = DEFAULT_END_MAX_REAL;
        end->min_real = DEFAULT_END_MIN_REAL;
    }

    double max_img_incr = (end->max_img - start->max_real) / NUM_FRAMES;
    double min_img_incr = (end->min_img - start->min_img) / NUM_FRAMES;
    double max_real_incr = (end->max_real - start->max_real) / NUM_FRAMES;
    double min_real_incr = (end->min_real - start->min_real) / NUM_FRAMES;
   
    double curr_max_img = start->max_img;
    double curr_min_img = start->min_img;
    double curr_max_real = start->max_real;
    double curr_min_real = start->min_real;

    double min_difference = NUM_FRAMES*fmin(fabs(min_real_incr), fmin(fabs(max_real_incr), fmin(fabs(max_img_incr), fabs(min_img_incr))));
    double log_difference = log(min_difference);
    double log_scale_factor = log(SCALE_FACTOR);
    double log_divided = log_difference / log_scale_factor;
    int bounds = (int) log_divided;
    fprintf(stderr, "%f : %f : %f : %f : %d\n", min_difference, log_difference, log_scale_factor, log_divided, bounds);

    int num_bounds = (int) (log(min_difference)/(log(SCALE_FACTOR)));
    
    assert( num_bounds > 0 );
    
    ComplexBounds** scene_bounds = (ComplexBounds**) calloc(num_bounds+2, sizeof(ComplexBounds*));
    scene_bounds[0] = start;
    
    for (int i = 0; i < num_bounds; i++) {
        ComplexBounds* curr_bounds = (ComplexBounds*) calloc(1, sizeof(ComplexBounds));
        assert(curr_bounds != NULL);
        curr_max_img += max_img_incr;
        curr_min_img += min_img_incr;
        curr_max_real += max_real_incr;
        curr_min_real += min_real_incr;
        
        curr_bounds->max_img = curr_max_img;
        curr_bounds->min_img = curr_min_img;
        curr_bounds->max_real = curr_max_real;
        curr_bounds->min_real = curr_min_real;

        max_img_incr *= SCALE_FACTOR;
        min_img_incr *= SCALE_FACTOR;
        max_img_incr *= SCALE_FACTOR;
        min_real_incr *= SCALE_FACTOR;
    }
    scene_bounds[num_bounds+1] = end;

    scene->scenes = scene_bounds;

    return scene;
}
