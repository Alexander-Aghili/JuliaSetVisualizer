#include <stdio.h>
#include "julia_set_cuda.h"
#include "perf_man.h"

int main() {
    ComplexBounds scene_bounds = { -2.0, 1.0, -1.5, 1.5 };
    ComplexNumber c = { -0.7, 0.27015 };

    uint32_t* image_pixels = create_image_pixels_arr(WIDTH, HEIGHT);

    // Initialize the timer
    init_timer(1, THREADS_PER_BLOCK);
    
    // Start the timer
    start_timer();

    // Perform the Mandelbrot set computation
    add_pixel(&scene_bounds, &c, image_pixels);

    // Stop the timer and write the results
    stop_timer_message("Mandelbrot computation");

    cudaFree(image_pixels);
    return 0;
}
