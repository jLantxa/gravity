#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <SDL2/SDL.h>

#include "gravity.hpp"
#include "Universe.hpp"
#include "Timer.hpp"
#include "Launcher.hpp"

struct GameState {
    bool run;
    bool pause;
    bool fieldView;
};

class Game {
private:
    Color BACKGROUND_COLOR = {0x00, 0x00, 0x00, 0xFF};

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
