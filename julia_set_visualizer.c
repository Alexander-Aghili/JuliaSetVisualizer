#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <inttypes.h>

#include "constants.h"
#include "color_utilities.h"
#include "graphics_utilities.h"
#include "complex.h"
#include "assert.h"

ComplexScene *create_complex_scene(ComplexNumber *c, ComplexBounds *start, ComplexBounds *end);
void generate_frames(ComplexScene *scene);

int main() {
    initialize_color_map(); 
    ComplexScene *scene = create_complex_scene(NULL, NULL, NULL);
    generate_frames(scene);
}

ComplexScene *create_complex_scene(ComplexNumber *c, ComplexBounds *start, ComplexBounds *end) {
    ComplexScene *scene = (ComplexScene *) calloc(1, sizeof(ComplexScene));
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

    double max_img_diff = (end->max_img - start->max_img);
    double min_img_diff = (end->min_img - start->min_img);
    double max_real_diff = (end->max_real - start->max_real);
    double min_real_diff = (end->min_real - start->min_real);

   double min_difference = fmin(fabs(min_real_diff),
        fmin(fabs(max_real_diff), fmin(fabs(max_img_diff), fabs(min_img_diff)))) / NUM_FRAMES;

    int num_bounds = (int) (log(min_difference) / (log(SCALE_FACTOR)));

    double max_img_incr = (end->max_img - start->max_img) / num_bounds;
    double min_img_incr = (end->min_img - start->min_img) / num_bounds;
    double max_real_incr = (end->max_real - start->max_real) / num_bounds;
    double min_real_incr = (end->min_real - start->min_real) / num_bounds;

    double curr_max_img = start->max_img;
    double curr_min_img = start->min_img;
    double curr_max_real = start->max_real;
    double curr_min_real = start->min_real;


    assert(num_bounds > 0);

    ComplexBounds **scene_bounds
        = (ComplexBounds **) calloc(num_bounds + 2, sizeof(ComplexBounds *));
    scene_bounds[0] = start;

    for (int i = 0; i < num_bounds; i++) {
        ComplexBounds *curr_bounds = (ComplexBounds *) calloc(1, sizeof(ComplexBounds));
        assert(curr_bounds != NULL);
        curr_max_img += max_img_incr;
        curr_min_img += min_img_incr;
        curr_max_real += max_real_incr;
        curr_min_real += min_real_incr;

        curr_bounds->max_img = curr_max_img;
        curr_bounds->min_img = curr_min_img;
        curr_bounds->max_real = curr_max_real;
        curr_bounds->min_real = curr_min_real;

        scene_bounds[i + 1] = curr_bounds;

    }
    scene_bounds[num_bounds + 1] = end;

    scene->scenes = scene_bounds;
    scene->num_scenes = num_bounds + 2;

    return scene;
}

double screen_map(
    double input_num, double min_input, double max_input, double min_output, double max_output) {
    return (input_num - min_input) * (max_output - min_output) / (max_input - min_input)
           + min_output;
}

void generate_frames(ComplexScene *scene) {
    for (int i = 0; i < scene->num_scenes; i++) {
        ComplexBounds *scene_bounds = scene->scenes[i];
        fprintf(stderr, "Scene %d (%f, %f)U(%f, %f)\n", i, scene_bounds->min_real, scene_bounds->max_real, scene_bounds->min_img, scene_bounds->max_img);
        uint32_t image_pixels[WIDTH][HEIGHT];
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                double a = screen_map(x, 0, WIDTH, scene_bounds->min_real, scene_bounds->max_real);
                double b = screen_map(y, 0, HEIGHT, scene_bounds->min_img, scene_bounds->max_img);
            
                int n = color_point(a, b);

                uint32_t color = get_color(n);
                image_pixels[x][y] = color;
            }
        }
        create_image(i, image_pixels);
    }
}
