#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <linux/limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "color_utilities.h"
#include "graphics_utilities.h"
#include "complex.h"
#include "assert.h"
#include "perf_man.h"
#include "helpers.h"

ComplexScene *create_complex_scene(ComplexNumber *c, ComplexBounds *start);
void show_julia_start(ComplexScene* scene);
void performance_test(ComplexScene* scene);

double get_wait_time();

static int num_print, num_chunks;

int main(int argc, char** argv) {
    argparser(argc, argv, &num_print, &num_chunks);

//    start_sdl();
    initialize_color_map();
    ComplexNumber* c = create_complex_number(0, 0);
    ComplexScene *scene = create_complex_scene(c, NULL);

//    show_julia_start(scene);
    performance_test(scene);
}

void wait() {
   sleep(WAIT); 
}

ComplexScene *create_complex_scene(ComplexNumber *c, ComplexBounds *start) {
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

    scene->bounds = start;

    return scene;
}

double screen_map(
    double input_num, double min_input, double max_input, double min_output, double max_output) {
    return (input_num - min_input) * (max_output - min_output) / (max_input - min_input)
           + min_output;
}

uint32_t** create_image_pixels_arr(int x, int y) {
    uint32_t **image_pixels = (uint32_t **)malloc(x * sizeof(uint32_t *));
    for (int i = 0; i < x; i++) {
        image_pixels[i] = (uint32_t *)malloc(y * sizeof(uint32_t));
    }
    return image_pixels;
}

void add_pixel(int x, int y, ComplexBounds* scene_bounds, ComplexNumber* c, uint32_t** image_pixels) {
    double a = screen_map(x, 0, WIDTH, scene_bounds->min_real, scene_bounds->max_real);
    double b = screen_map(y, 0, HEIGHT, scene_bounds->min_img, scene_bounds->max_img);
    
    int n = color_point(a, b, c);
    uint32_t color = get_color(n);
    image_pixels[x][y] = color;
}


typedef struct {
    uint32_t** image_pixels;
    int x_chunk;
    int y_chunk;
    ComplexScene* scene;
} ChunkData;

void get_chunk_bounds(int screen_width, int screen_height, int num_chunks, int chunk_row, int chunk_col, int *x_min, int *y_min, int *x_max, int *y_max) {
    // Calculate the width and height of each chunk
    int chunk_width = screen_width / num_chunks;
    int chunk_height = screen_height / num_chunks;

    // Calculate the bounds of the specified chunk
    *x_min = chunk_col * chunk_width;
    *x_max = *x_min + chunk_width;
    *y_min = chunk_row * chunk_height;
    *y_max = *y_min + chunk_height;
}

void* calculate_chunk(void* d) {
   ChunkData* data = (ChunkData*) d; 
   int x_min, x_max, y_min, y_max; 
   get_chunk_bounds(WIDTH, HEIGHT, num_chunks, data->x_chunk, data->y_chunk, &x_min, &y_min, &x_max, &y_max);
   for (int x = x_min; x < x_max; x++) {
       for (int y = y_min; y < y_max; y++) {
           add_pixel(x, y, data->scene->bounds, data->scene->c, data->image_pixels);
       }
   }

   return NULL;
}

void run_chunk(pthread_t* tid, uint32_t** ip, int x_chunk, int y_chunk, ComplexScene* scene) {
    ChunkData* data = (ChunkData*) calloc(1, sizeof(ChunkData));
    data->image_pixels = ip;
    data->x_chunk = x_chunk;
    data->y_chunk = y_chunk;
    data->scene = scene;

    pthread_create(tid, NULL, calculate_chunk, data);
}

void calculate_pixels(ComplexScene* scene, uint32_t*** ip) {
    uint32_t ** image_pixels = *(ip);
    pthread_t tid[num_chunks * num_chunks];
    int k = 0;
    for (int x_chunk = 0; x_chunk < num_chunks; x_chunk++) {
        for (int y_chunk = 0; y_chunk < num_chunks; y_chunk++) {
            run_chunk(&tid[k], image_pixels, x_chunk, y_chunk, scene);
            k++;
        }
    }

    for (int k = 0; k < num_chunks*num_chunks; k++) {
        pthread_join(tid[k], NULL);
    }
    return;
}

void wait_event(ComplexNumber *c, int* change, int* quit) {
    SDL_Event event;
    struct timeval init;
        gettimeofday(&init,NULL);
        signed long init_time = 1000000 * init.tv_sec + init.tv_usec; 
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *quit = 1;
                break;
            } else if (event.key.keysym.sym == SDLK_LEFT) {
                c->x -= 0.01;
                *change = 1;
            } else if (event.key.keysym.sym == SDLK_RIGHT) {
                c->x += 0.01;
                *change = 1;
            } else if (event.key.keysym.sym == SDLK_UP) {
                c->y += 0.01;
                *change = 1;
            } else if (event.key.keysym.sym == SDLK_DOWN) {
                c->y -= 0.01;
                *change = 1;
            }
            struct timeval curr;
            gettimeofday(&curr,NULL);
            signed long curr_time = 1000000 * curr.tv_sec + curr.tv_usec; 
            if (curr_time-init_time >= 500) {
                break;
            }
        }
}

void show_julia_start(ComplexScene* scene) {
    int quit = 0;
    int change = 1;

    uint32_t** image_pixels = create_image_pixels_arr(WIDTH, HEIGHT);
    while (!quit) {
        if (change) {
            start_timer();
            calculate_pixels(scene, &image_pixels);
            char data[100];
            //sprintf(data, "(%.10f, %.10f)U(%.10f, %.10f)", scene->bounds->min_real, scene->bounds->max_real, scene->bounds->min_img, scene->bounds->max_img);
            sprintf(data, "(%.10f, %.10f)", scene->c->x, scene->c->y);
            stop_timer_message(data);
            display_image(image_pixels);    
            change = 0;
        }
       wait_event(scene->c, &change, &quit); 
    }
}

#define NUM_MOVEMENTS 20

void performance_test(ComplexScene* scene) {
    uint32_t** image_pixels = create_image_pixels_arr(WIDTH, HEIGHT);
    scene->c->x = 0;
    init_timer(num_print, num_chunks);
    for (int i = 0; i < NUM_MOVEMENTS; i++) {
        start_timer();
        calculate_pixels(scene, &image_pixels);
        char data[100];
        sprintf(data, "(%.10f, %.10f)", scene->c->x, scene->c->y);
        stop_timer_message(data);
        scene->c->x -= .01;
    }
}
