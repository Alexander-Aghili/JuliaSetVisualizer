#include <cuda_runtime.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h> 
#include <unistd.h>
#include "julia_set_cuda.h"

extern "C" {
    #include "perf_man.h"
    #include "complex.h"
    #include "graphics_utilities.h"
}

// Precomputed color values
__device__ uint32_t color_map[16] = {
    0x00421E0F, // get_int_from_color(66, 30, 15)
    0x0019071A, // get_int_from_color(25, 7, 26)
    0x0009012F, // get_int_from_color(9, 1, 47)
    0x00040449, // get_int_from_color(4, 4, 73)
    0x00000764, // get_int_from_color(0, 7, 100)
    0x000C2C8A, // get_int_from_color(12, 44, 138)
    0x001852B1, // get_int_from_color(24, 82, 177)
    0x003987D1, // get_int_from_color(57, 135, 209)
    0x0086B5E5, // get_int_from_color(134, 181, 229)
    0x00D3ECF8, // get_int_from_color(211, 236, 248)
    0x00F1E9BF, // get_int_from_color(241, 233, 191)
    0x00F8C95F, // get_int_from_color(248, 201, 95)
    0x00FFAA00, // get_int_from_color(255, 170, 0)
    0x00CC8000, // get_int_from_color(204, 128, 0)
    0x00995700, // get_int_from_color(153, 87, 0)
    0x006A3403  // get_int_from_color(106, 52, 3)
};

__device__ uint32_t black = 0x00000000;

__device__ uint32_t get_color(int n) {
    if (n < MAX_ITERATIONS && n > 0) {
        int i = n % 16;
        return color_map[i];
    }
    return black;
}

__device__ int color_point(double a, double b, ComplexNumber* c) {
    int n = 0;
    while (n < MAX_ITERATIONS) {
        double u = (a * a - b * b) + c->x;
        double v = (2 * a * b) + c->y;

        if (fabs(u + v) > THRESHOLD) {
            break;
        }

        a = u;
        b = v;

        n++;
    }
    return n;
}

__device__ double screen_map(double input_num, double min_input, double max_input, double min_output, double max_output) {
    return (input_num - min_input) * (max_output - min_output) / (max_input - min_input) + min_output;
}

__global__ void add_pixel_kernel(ComplexBounds* scene_bounds, ComplexNumber* c, uint32_t* image_pixels) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < WIDTH && y < HEIGHT) {
        double a = screen_map(x, 0, WIDTH, scene_bounds->min_real, scene_bounds->max_real);
        double b = screen_map(y, 0, HEIGHT, scene_bounds->min_img, scene_bounds->max_img);

        int n = color_point(a, b, c);
        uint32_t color = get_color(n);
        image_pixels[y * WIDTH + x] = color;  // Note: linearized indexing for 2D array
    }
}

void add_pixel(ComplexBounds* scene_bounds, ComplexNumber* c, uint32_t* image_pixels) {
    dim3 threadsPerBlock(THREADS_PER_BLOCK, THREADS_PER_BLOCK);
    dim3 numBlocks((WIDTH + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK, (HEIGHT + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK);

    add_pixel_kernel<<<numBlocks, threadsPerBlock>>>(scene_bounds, c, image_pixels);
    cudaDeviceSynchronize();
}

uint32_t* create_image_pixels_arr(int x, int y) {
    uint32_t *image_pixels;
    cudaMallocManaged(&image_pixels, x * y * sizeof(uint32_t));
    return image_pixels;
}

#define NUM_MOVEMENTS 20

void show_julia_start(ComplexScene* scene);
ComplexScene *create_complex_scene(ComplexNumber *c, ComplexBounds *start);

int main(int argc, char** argv) {
    start_sdl();
    ComplexNumber* number = (ComplexNumber*) calloc(1, sizeof(ComplexNumber*));
    ComplexScene *scene = create_complex_scene(number, NULL);

    show_julia_start(scene);
}

ComplexBounds* get_start_bounds() {
    ComplexBounds* start = (ComplexBounds*) calloc(1, sizeof(ComplexBounds));
    assert(start != NULL);
    start->max_img = DEFAULT_START_MAX_IMG;
    start->min_img = DEFAULT_START_MIN_IMG;
    start->max_real = DEFAULT_START_MAX_REAL;
    start->min_real = DEFAULT_START_MIN_REAL;
    return start;

}

ComplexScene *create_complex_scene(ComplexNumber *c, ComplexBounds *start) {
    ComplexScene *scene = (ComplexScene *) calloc(1, sizeof(ComplexScene));
    assert(scene != NULL);
    scene->c = c;


    if (start == NULL) {
        start = get_start_bounds();
    }

    scene->bounds = start;

    return scene;
}

void zoom(ComplexBounds* bounds, double scaling_factor) {
    double real_center = (bounds->max_real + bounds->min_real)/2;
    double img_center = (bounds->max_img + bounds->min_img)/2;
    bounds->max_real = real_center + scaling_factor * (bounds->max_real - real_center);
    bounds->min_real = real_center + scaling_factor * (bounds->min_real - real_center);
    bounds->max_img = img_center + scaling_factor * (bounds->max_img - img_center);
    bounds->min_img = img_center + scaling_factor * (bounds->min_img - img_center);
}

int wait_event(ComplexScene *scene, int* change, int* quit) {
    SDL_Event event;
    ComplexNumber* c = scene->c;
    ComplexBounds* bounds = scene->bounds;
    int ret = 0;
    struct timeval init;
    gettimeofday(&init,NULL);
    signed long init_time = 1000000 * init.tv_sec + init.tv_usec; 
    double move_amount = 0.005;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *quit = 1;
            break;
        } else if (event.key.keysym.sym == SDLK_LEFT) {
            c->x -= move_amount;
            *change = 1;
        } else if (event.key.keysym.sym == SDLK_RIGHT) {
            c->x += move_amount;
            *change = 1;
        } else if (event.key.keysym.sym == SDLK_UP) {
            c->y += move_amount;
            *change = 1;
        } else if (event.key.keysym.sym == SDLK_DOWN) {
            c->y -= move_amount;
            *change = 1;
        } else if (event.button.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int x1, y1, x2, y2;
            SDL_GetMouseState(&x1, &y1);

            SDL_Event mouse_up;
            mouse_up.button.type = SDL_MOUSEBUTTONUP;
            while (SDL_WaitEvent(&mouse_up)) {
                if (mouse_up.button.type == SDL_MOUSEBUTTONUP) {
                    break;
                }
            }

            SDL_GetMouseState(&x2, &y2);

            double xdiff = ((double)(x1 - x2)/(sqrt((double)(x1*x1)+(double)(x2*x2))))*((bounds->max_real-bounds->min_real)/**(bounds->max_img-bounds->min_img)*/);
            double ydiff = ((double)(y1 - y2)/(sqrt((double)(y1*y1)+(double)(y2*y2))))*((bounds->max_real-bounds->min_real)/**(bounds->max_img-bounds->min_img)*/);

            bounds->max_real += xdiff;
            bounds->min_real += xdiff;
            bounds->max_img += ydiff;
            bounds->min_img += ydiff;
            *change = 1;
        } else if (event.key.keysym.sym == SDLK_EQUALS) {
            zoom(bounds, 0.9);
            *change = 1;
        } else if (event.key.keysym.sym == SDLK_MINUS) {
            zoom(bounds, 1.1);
            *change = 1;
        } else if (event.key.keysym.sym == SDLK_RETURN) {
            printf("(%f, %f, %f, %f)\n", bounds->max_real, bounds->min_real, bounds->max_img, bounds->min_img);
            ret = 1;
        }
        /*            
                      struct timeval curr;
                      gettimeofday(&curr,NULL);
                      signed long curr_time = 1000000 * curr.tv_sec + curr.tv_usec; 
                      if (curr_time-init_time >= 500) {
                      break;
                      }
         */
    }
    return ret;
}

void zoom_display(ComplexScene* scene);

void show_julia_start(ComplexScene* scene) {
    int quit = 0;
    int change = 1;

    uint32_t* image_pixels = create_image_pixels_arr(WIDTH, HEIGHT);
    while (!quit) {
        if (change) {
            add_pixel(scene->bounds, scene->c, image_pixels);
            display_image(image_pixels);    
            change = 0;
        }
       int ret = wait_event(scene, &change, &quit); 
       if (ret == 1) {
           //Zoom 
           zoom_display(scene);
       }
    }
}

void zoom_display(ComplexScene* scene) {
    ComplexBounds* start = get_start_bounds(); 
    ComplexBounds* end = scene->bounds;

    int frames = 10;
    double max_img_incr = (end->max_img - start->max_img) / frames;
    double min_img_incr = (end->min_img - start->min_img) / frames;
    double max_real_incr = (end->max_real - start->max_real) / frames;
    double min_real_incr = (end->min_real - start->min_real) / frames;

    double curr_max_img = start->max_img;
    double curr_min_img = start->min_img;
    double curr_max_real = start->max_real;
    double curr_min_real = start->min_real;

    uint32_t** image_list = (uint32_t**) calloc(1000, sizeof(uint32_t*));
    
    int i = 0;
    while (fabs(curr_max_img - end->max_img) > EPSILON
           || fabs(curr_max_real - end->max_real) > EPSILON
           || fabs(curr_min_img - end->min_img) > EPSILON
           || fabs(curr_min_real - end->min_real) > EPSILON) {
        
        uint32_t* image_pixels = create_image_pixels_arr(WIDTH, HEIGHT);
        image_list[i++] = image_pixels;

        add_pixel(start, scene->c, image_pixels);

        curr_max_img += max_img_incr;
        curr_min_img += min_img_incr;
        curr_max_real += max_real_incr;
        curr_min_real += min_real_incr;

        start->max_img = curr_max_img;
        start->min_img = curr_min_img;
        start->max_real = curr_max_real;
        start->min_real = curr_min_real;
           
        max_img_incr *= SCALE_FACTOR;
        min_img_incr *= SCALE_FACTOR;
        max_real_incr *= SCALE_FACTOR;
        min_real_incr *= SCALE_FACTOR;
    }

    for (int k = 0; k < i; k++) {
        display_image(image_list[k]);
        usleep(100);
    }

}
