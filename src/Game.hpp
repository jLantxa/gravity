#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <SDL2/SDL.h>

#include "gravity.hpp"
#include "Universe.hpp"
#include "Timer.hpp"

struct GameState {
    bool run;
    bool pause;
    bool fieldView;
    bool antimatter;
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
    Color BACKGROUND_COLOR = {200, 200, 200, 255};

    GameState mGameState;
    Timer mTimer;

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    bool bFullScreen = false;
    bool toggleFullScreen();
    bool togglePause();

    Universe mUniverse;
    Launcher mLauncher;
    void drawLauncher();

    void handle_events();

    void setRenderColor(Color color);

    int mFieldViewSubsambpleX;
    int mFieldViewSubsambpleY;
    void renderField(int subsample_x, int subsample_y);

    void eraseOOBParticles();

public:
    Game();
    virtual ~Game();

    int init(window_params* wparams);
    int run();

    void quit();
    void pause(bool state);
};

#endif
