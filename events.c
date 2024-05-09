#include "events.h"

void key_event(ComplexNumber** c) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_LEFT]) {
        (*c)->x -= 0.1;
    }
    if (keystate[SDL_SCANCODE_RIGHT]) {
        (*c)->x += 0.1;
    }
    if (keystate[SDL_SCANCODE_UP]) {
        (*c)->y += 0.1;
    }
    if (keystate[SDL_SCANCODE_DOWN]) {
        (*c)->y -= 0.1;
    }
}

