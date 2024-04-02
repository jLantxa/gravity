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

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <SDL2/SDL.h>

#include "Launcher.hpp"
#include "Timer.hpp"
#include "Universe.hpp"
#include "gravity.hpp"

class Game {
public:
  struct GameState {
    bool run;
    bool pause;
    bool fieldView;
  };

  Game();
  virtual ~Game();

  int init(window_params *wparams);
  int run();

  void quit();
  void pause(bool state);

private:
  Color BACKGROUND_COLOR = {0x00, 0x00, 0x00, 0xFF};

  GameState mGameState;
  Timer mTimer;

  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;
  bool bFullScreen = false;
  bool toggleFullScreen();

  Universe mUniverse;
  Launcher mLauncher;
  void drawLauncher();

  void handle_events();

  void setRenderColor(Color color);

  int mFieldViewSubsambpleX;
  int mFieldViewSubsambpleY;
  void renderField(int subsample_x, int subsample_y);

  void eraseOOBParticles();
};

#endif
