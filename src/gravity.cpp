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

#include "gravity.hpp"

#include "Game.hpp"
#include "log.hpp"

int main(int argc, char *argv[]) {
  int ret = 0;
  Game game;

  // Fill window params
  window_params wparams{.title = constants::WINDOW_TITLE.c_str(),
                        .width = constants::WINDOW_WIDTH,
                        .height = constants::WINDOW_HEIGHT};

  ret = game.init(&wparams);
  if (ret != 0) {
    return ret;
  }

  LOGV("Running game...\n");
  ret = game.run();

  // End of loop

  LOGD("Exit with return code %d\n", ret);
  return ret;
}
