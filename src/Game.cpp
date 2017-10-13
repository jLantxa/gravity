#include "SDL2/SDL.h"

#include "log.hpp"
#include "Game.hpp"
#include "Universe.hpp"

Game::Game(SDL_Window* window) :
    mWindow(window),
    mSurface(SDL_GetWindowSurface(mWindow)),
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
    int window_width, window_height;
    SDL_GetWindowSize(mWindow, &window_width, &window_height);
    mUniverse.addStar(window_width/2, window_height/2);

    // Game loop
    while (mGameState.run == true) {
        handle_events();
        mUniverse.update();

        // TODO: Paint screen surface
        for (auto p = mUniverse.particles().begin(); p < mUniverse.particles().end(); p++) {
            // DRAW
        }

        SDL_UpdateWindowSurface(mWindow);
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
