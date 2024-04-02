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

#include "Launcher.hpp"

Launcher::Launcher() { clicked = false; }

Launcher::~Launcher() {}

void Launcher::click(int x, int y) {
  clicked = true;
  start_x = x;
  start_y = y;
  end_x = x;
  end_y = y;
}

void Launcher::move(int x, int y) {
  end_x = x; // Segment length
  end_y = y; // Segment length
}

void Launcher::release() { clicked = false; }
