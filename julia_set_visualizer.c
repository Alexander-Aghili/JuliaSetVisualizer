#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <linux/limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "color_utilities.h"
#include "graphics_utilities.h"
#include "complex.h"
#include "assert.h"
#include "events.h"

ComplexScene *create_complex_scene(ComplexNumber *c, ComplexBounds *start, ComplexBounds *end);
void generate_frames(ComplexScene *scene);
void display_scene(ComplexScene* scene);
void show_julia_start(ComplexScene* scene);

double get_wait_time();

int main() {
    start_sdl();
    initialize_color_map();
    ComplexNumber* c = create_complex_number(0, 0);
    ComplexScene *scene = create_complex_scene(c, NULL, NULL);

//    generate_frames(scene);
//    display_scene(scene);
      show_julia_start(scene);
}

void wait() {
   sleep(WAIT); 
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

    int num_bounds = NUM_FRAMES;

    double max_img_incr = (end->max_img - start->max_img) / NUM_FRAMES;
    double min_img_incr = (end->min_img - start->min_img) / NUM_FRAMES;
    double max_real_incr = (end->max_real - start->max_real) / NUM_FRAMES;
    double min_real_incr = (end->min_real - start->min_real) / NUM_FRAMES;

    double curr_max_img = start->max_img;
    double curr_min_img = start->min_img;
    double curr_max_real = start->max_real;
    double curr_min_real = start->min_real;

    assert(num_bounds > 0);

    ComplexBounds **scene_bounds = (ComplexBounds **) calloc(num_bounds, sizeof(ComplexBounds *));
    scene_bounds[0] = start;
    scene->num_scenes = 1;

    int i = 1;
    while (fabs(curr_max_img - end->max_img) > EPSILON
           || fabs(curr_max_real - end->max_real) > EPSILON
           || fabs(curr_min_img - end->min_img) > EPSILON
           || fabs(curr_min_real - end->min_real) > EPSILON) {
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

        scene_bounds[i++] = curr_bounds;
        fprintf(stderr, "Scene %d (%f, %f)U(%f, %f)\n", i - 1, scene_bounds[i - 1]->min_real,
            scene_bounds[i - 1]->max_real, scene_bounds[i - 1]->min_img,
            scene_bounds[i - 1]->max_img);
        scene->num_scenes++;

        if (scene->num_scenes >= num_bounds - 1) {
            scene_bounds = realloc(scene_bounds, (num_bounds * 2) * sizeof(ComplexBounds *));
            num_bounds *= 2;
        }

        max_img_incr *= SCALE_FACTOR;
        min_img_incr *= SCALE_FACTOR;
        max_real_incr *= SCALE_FACTOR;
        min_real_incr *= SCALE_FACTOR;
    }
    scene->scenes = scene_bounds;

    uint32_t ***image_scenes = (uint32_t***) calloc(scene->num_scenes, sizeof(uint32_t**));
    scene->image_scenes = image_scenes;

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
        fprintf(stderr, "Scene %d (%f, %f)U(%f, %f)\n", i, scene_bounds->min_real,
            scene_bounds->max_real, scene_bounds->min_img, scene_bounds->max_img);
        uint32_t image_pixels[WIDTH][HEIGHT];
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                double a = screen_map(x, 0, WIDTH, scene_bounds->min_real, scene_bounds->max_real);
                double b = screen_map(y, 0, HEIGHT, scene_bounds->min_img, scene_bounds->max_img);

                int n = color_point(a, b, scene->c);

                uint32_t color = get_color(n);
                image_pixels[x][y] = color;
            }
        }
        create_image(i, image_pixels);
    }
}

void display_scene(ComplexScene* scene) {
    for (int i = 0; i < scene->num_scenes; i++) {
        ComplexBounds *scene_bounds = scene->scenes[i];
        fprintf(stderr, "Scene %d (%f, %f)U(%f, %f)\n", i, scene_bounds->min_real,
            scene_bounds->max_real, scene_bounds->min_img, scene_bounds->max_img);
        uint32_t **image_pixels = (uint32_t **)malloc(WIDTH * sizeof(uint32_t *));
        for (int i = 0; i < WIDTH; i++) {
             image_pixels[i] = (uint32_t *)malloc(HEIGHT* sizeof(uint32_t));
        }

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                double a = screen_map(x, 0, WIDTH, scene_bounds->min_real, scene_bounds->max_real);
                double b = screen_map(y, 0, HEIGHT, scene_bounds->min_img, scene_bounds->max_img);

                int n = color_point(a, b, scene->c);

                uint32_t color = get_color(n);
                image_pixels[x][y] = color;
            }
        }
        scene->image_scenes[i] = image_pixels;
        break;
    }



    for (int i = 0; i < scene->num_scenes; i++) {
        display_image(scene->image_scenes[i]);    
        scene_complete();
        wait();
    }
}

uint32_t** create_image_pixels_arr(int x, int y) {
    uint32_t **image_pixels = (uint32_t **)malloc(x * sizeof(uint32_t *));
    for (int i = 0; i < x; i++) {
        image_pixels[i] = (uint32_t *)malloc(y * sizeof(uint32_t));
    }
    return image_pixels;
}

void calculate_pixels(ComplexScene* scene, int i, uint32_t*** ip) {
    uint32_t ** image_pixels = *(ip);
    ComplexBounds *scene_bounds = scene->scenes[i];
    for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                double a = screen_map(x, 0, WIDTH, scene_bounds->min_real, scene_bounds->max_real);
                double b = screen_map(y, 0, HEIGHT, scene_bounds->min_img, scene_bounds->max_img);

                int n = color_point(a, b, scene->c);

                uint32_t color = get_color(n);
                image_pixels[x][y] = color;
            }
    }
    return;
}

void show_julia_start(ComplexScene* scene) {
    int quit = 0;
    SDL_Event event;

    uint32_t** image_pixels = create_image_pixels_arr(WIDTH, HEIGHT);
    while (!quit) {
        fprintf(stderr, "%f + %fi\n", scene->c->x, scene->c->y);
        calculate_pixels(scene, 0, &image_pixels);
        display_image(image_pixels);    
        while (SDL_WaitEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
                break;
            } else if (event.key.keysym.sym == SDLK_LEFT) {
                scene->c->x -= 0.1;
                break;
            } else if (event.key.keysym.sym == SDLK_RIGHT) {
                scene->c->x += 0.1;
                break;
            } else if (event.key.keysym.sym == SDLK_UP) {
                scene->c->y += 0.1;
                break;
            } else if (event.key.keysym.sym == SDLK_DOWN) {
                scene->c->y -= 0.1;
                break;
            }
        }

    }
}
