#include <cuda_runtime.h>
#include <stdint.h>
#include <math.h>
#include "julia_set_cuda.h"


__device__ uint32_t color_map[16] = {0x000000, 0x1F1F1F, 0x3F3F3F, 0x5F5F5F, 0x7F7F7F, 0x9F9F9F, 0xBFBFBF, 0xDFDFDF,
                                     0xFF0000, 0xFF7F00, 0xFFFF00, 0x00FF00, 0x0000FF, 0x4B0082, 0x8F00FF, 0xFFFFFF};
__device__ uint32_t black = 0x000000;

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

int main() {
    ComplexBounds scene_bounds = { -2.0, 1.0, -1.5, 1.5 };
    ComplexNumber c = { -0.7, 0.27015 };

    uint32_t* image_pixels = create_image_pixels_arr(WIDTH, HEIGHT);

    add_pixel(&scene_bounds, &c, image_pixels);

    cudaFree(image_pixels);
    return 0;
}
