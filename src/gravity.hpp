/*
 * gravity
 * Copyright (C) 2017 - 2020  Javier Lancha Vázquez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
