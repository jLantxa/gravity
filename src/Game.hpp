#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "SDL2/SDL.h"

#include "Universe.hpp"

struct game_state {
    bool run;
    bool pause;
};

class Game {
private:
    game_state mGameState;
    SDL_Window* mWindow;
    SDL_Surface* mSurface;

    Universe mUniverse;
    void draw_circle(int x, int y, int radius, SDL_Surface *thescreen, int r, int g, int b);

    void handle_events();

public:
    Game(SDL_Window* window);
    virtual ~Game();

    int initSDL();
    int run();

    void quit();
    void pause(bool state);
};

#endif
