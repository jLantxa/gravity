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
    int start_x;
    int start_y;
    int end_x;
    int end_y;
};

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};

class Game {
private:
    Color BACKGROUND_COLOR = {0xFF, 0xFF, 0xFF, 0xFF};

    GameState mGameState;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    Universe mUniverse;
    Launcher mLauncher;
    void drawLauncher();

    void handle_events();

    void setRenderColor(Color color);

public:
    Game();
    virtual ~Game();

    int init(window_params* wparams);
    int run();

    void quit();
    void pause(bool state);
};

#endif
