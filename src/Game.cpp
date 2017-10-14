#include "SDL2/SDL.h"

#include "log.hpp"
#include "gravity.hpp"
#include "Timer.hpp"
#include "Game.hpp"
#include "Universe.hpp"

Game::Game() :
    mWindow(NULL),
    mRenderer(NULL),
    mUniverse()
{
    LOGVV("%s:\tGame constructor\n", __func__);
}

Game::~Game() {
    LOGVV("%s:\tGame destructor\n", __func__);
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

int Game::init(window_params* wparams) {
    LOGI("Init Game\n");

	//Initialization flag
	int ret = 0;

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOGE("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		ret = -1;
	}
	else
	{
        mWindow = SDL_CreateWindow (
                wparams->title,             // title
                SDL_WINDOWPOS_UNDEFINED,    // initial x
                SDL_WINDOWPOS_UNDEFINED,    // initial y
                wparams->width,             // width
                wparams->height,            // height
                SDL_WINDOW_FULLSCREEN_DESKTOP
        );

        if (mWindow == NULL) {
            LOGE("%s:\t Could not create window: %s\n", __func__, SDL_GetError());
            return -1;
        }

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			LOGE("Warning: Linear texture filtering not enabled!");
		}

        //Create renderer for window
		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
		if (mRenderer == NULL) {
			LOGE("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			ret = -1;
		}
	}

    mGameState.run =  true;
    mGameState.pause = false;

	return ret;
}

void Game::quit() {
    mGameState.run = false;
}

void Game::pause(bool state) {
    mGameState.pause = state;
}

void Game::drawLauncher() {
    
}

int Game::run() {
    long frame = 0;
    int window_width, window_height;
    SDL_GetWindowSize(mWindow, &window_width, &window_height);
    mUniverse.addStar(window_width/2, window_height/2);
    mUniverse.addPlanet(window_width/2 + 6*STAR_R*2, window_height/2, 0.0, -5.0);

    // Game loop
    Timer fps;
    fps.init();
    while (mGameState.run == true) {
        fps.start();
        ++frame;

        /* ======== EVENTS ======== */
        handle_events();

        /* ======== LOGIC ======== */
        mUniverse.update();

        /* ======== DRAW ======== */
        // Clear screen
        SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(mRenderer);

        // Draw particles
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        for (auto p = mUniverse.particles().begin(); p < mUniverse.particles().end(); p++) {
            SDL_Rect fillRect = {
                p->x - p->r,
                p->y - p->r,
                2*p->r,
                2*p->r
            };

            //SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(mRenderer, &fillRect);
        }

        // Draw launcher
        drawLauncher();

        SDL_RenderPresent(mRenderer);

        //Sleep until next frame remaining frame time
        int slack = (1000 / FRAMES_PER_SECOND) - fps.get_ticks();
        if (slack > 0) {
            int slack = (1000 / FRAMES_PER_SECOND) - fps.get_ticks();
            LOGVV("Frame %d, slack %d\n", frame, slack);
            SDL_Delay(slack);
        } else {
            LOGE("Negative slack (%d ms) after frame %d\n", slack, frame);
        }
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

        case SDL_KEYDOWN:
            LOGD("%s:\tSDL_KEYDOWN event\n", __func__);
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                LOGD("%s:\tPressed ESCAPE -> set mGameState.run = false\n", __func__);
                mGameState.run = false;
            }
            break;

        case SDL_MOUSEBUTTONDOWN: {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mLauncher.clicked = true;
                SDL_GetMouseState(&mLauncher.click_x, &mLauncher.click_y);
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (mLauncher.clicked == false) break;

                mLauncher.clicked = false;

                mUniverse.addPlanet(
                    mLauncher.click_x,
                    mLauncher.click_y,
                    0.0,//mLauncher.x - mLauncher.click_x,
                    0.0);//mLauncher.click_y - mLauncher.y);
            }
            break;
        }

        default:
            break;
        }
    }
}
