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

#ifndef _LAUNCHER_HPP_
#define _LAUNCHER_HPP_

class Launcher {
 private:
  bool clicked;
  int start_x;
  int start_y;
  int end_x;
  int end_y;

 public:
  Launcher();
  ~Launcher();

  void click(int x, int y);
  void move(int x, int y);
  void release();

  bool isClicked() { return clicked; }
  int getStartX() { return start_x; }
  int getStartY() { return start_y; }
  int getEndX() { return end_x; }
  int getEndY() { return end_y; }
};

#endif
