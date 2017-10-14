#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "SDL2/SDL.h"

#include "gravity.hpp"
#include "Universe.hpp"

struct GameState {
    bool run;
    bool pause;
};

struct Launcher {
    bool clicked;
    int click_x;
    int click_y;
    float x;
    float y;
};

class Game {
private:
    GameState mGameState;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    Universe mUniverse;
    Launcher mLauncher;
    void drawLauncher();

    void handle_events();

public:
    Game();
    virtual ~Game();

    int init(window_params* wparams);
    int run();

    void quit();
    void pause(bool state);
};

#endif
