/*
 * gravity
 * Copyright (C) 2017 - 2024  Javier Lancha Vázquez
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

#include <string>

#include <SDL2/SDL.h>

namespace constants {
const std::string WINDOW_TITLE{"Gravity"};
const std::string WINDOW_TITLE_PAUSE{"Gravity - PAUSED"};
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

constexpr int FRAMES_PER_SECOND = 60;
} // namespace constants

struct window_params {
  const char *title;
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
