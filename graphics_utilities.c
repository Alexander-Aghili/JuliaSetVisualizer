#include "graphics_utilities.h"

static SDL_Window *window;
static SDL_Renderer *renderer; 
 

void start_sdl() {
    int init = SDL_Init(SDL_INIT_VIDEO);
    assert(init == 0);
    
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Julia Set");
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

uint8_t red(uint32_t color) {
    return (uint8_t) ((color >> 16) & 0x000000FF);
}

uint8_t green(uint32_t color) {
    return (uint8_t) ((color >> 8) & 0x000000FF);
}

uint8_t blue(uint32_t color) {
    return (uint8_t) (color & 0x000000FF);
}

void display_image(uint32_t* image_pixels) {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            uint32_t color = image_pixels[y * WIDTH + x];
            SDL_SetRenderDrawColor(renderer, red(color), green(color), blue(color), 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);
}

void scene_complete() {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/*
void create_image(int f, uint32_t image_pixels[WIDTH][HEIGHT]) {
    gdImagePtr im = gdImageCreateTrueColor(WIDTH, HEIGHT);
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            gdImageSetPixel(im, x, y, image_pixels[x][y]);
        }
    }

    char file_name[MAX_DIGITS + SUFFIX];
    sprintf(file_name, "%d.png", f);

    FILE *img = fopen(file_name, "wb");
    gdImagePng(im, img);
    fclose(img);

    gdImageDestroy(im);
}*/
