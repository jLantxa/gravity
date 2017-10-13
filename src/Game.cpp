#include "SDL2/SDL.h"
#include <iostream>

#include "log.hpp"
#include "Game.hpp"
#include "Universe.hpp"

Game::Game(SDL_Surface* screenSurface) :
    mScreenSurface(screenSurface),
    mUniverse()
{
    LOGVV("%s:\tGame constructor\n", __func__);
    mGameState.run =  true;
    mGameState.pause = false;
}

Game::~Game() {
    LOGVV("%s:\tGame destructor\n", __func__);
}

void Game::quit() {
    mGameState.run = false;
}

void Game::pause(bool state) {
    mGameState.pause = state;
}

int Game::run() {
    // Game loop
    while (mGameState.run == true) {
        handle_events();
    }

    return 0;
}

void Game::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        LOGVV("%s:\tEvent type = %d\n", __func__, event.type);
        switch(event.type)
        {
        case SDL_QUIT:
            LOGV("%s:\tSDL_QUIT event\n", __func__);
            mGameState.run = false;
            break;

        default:
            break;
        }
    }
}
