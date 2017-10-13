#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "SDL2/SDL.h"

struct game_state {
    bool run;
    bool pause;
};

class Game {
private:
    game_state mGameState;
    SDL_Surface* mScreenSurface;

    void handle_events();

public:
    Game(SDL_Surface* screenSurface);
    virtual ~Game();

    int initSDL();
    int run();

    void quit();
    void pause(bool state);
};

#endif
