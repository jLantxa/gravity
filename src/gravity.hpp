#ifndef _GRAVITY_HPP_
#define _GRAVITY_HPP_

#include <SDL2/SDL.h>

#define WINDOW_TITLE "Gravity"
#define WINDOW_TITLE_PAUSE "Gravity - PAUSED"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define FRAMES_PER_SECOND 60

struct window_params {
    char* title;
    int width;
    int height;
};

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};

#endif
