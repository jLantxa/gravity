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
    mLauncher.clicked = false;
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

void Game::setRenderColor(Color color) {
    SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
}

void Game::drawLauncher() {
    if (!mLauncher.clicked) return;

    // Blue
    Color launcherColor = {
        0xFF - BACKGROUND_COLOR.r,
        0xFF - BACKGROUND_COLOR.g,
        0xFF - BACKGROUND_COLOR.b,
        0xFF,
    };

    setRenderColor(launcherColor);
    SDL_RenderDrawLine(mRenderer,
            mLauncher.start_x,
            mLauncher.start_y,
            2*mLauncher.start_x - mLauncher.end_x,  // Segment length
            2*mLauncher.start_y - mLauncher.end_y); // Segment length
}

int Game::run() {
    long frame = 0;
    int window_width, window_height;
    SDL_GetWindowSize(mWindow, &window_width, &window_height);

    #ifdef BLACK_HOLE_ON_CREATE
    mUniverse.addBlackHole(window_width/2, window_height/2);
    #endif

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
        setRenderColor(BACKGROUND_COLOR);
        SDL_RenderClear(mRenderer);

        // Draw particles
        Color particleColor;
        for (auto p = mUniverse.particles().begin(); p < mUniverse.particles().end(); p++) {
            SDL_Rect fillRect = {
                p->x - p->r,
                p->y - p->r,
                2*p->r,
                2*p->r
            };

            switch (p->type) {
                case PARTICLE_BLACK_HOLE:
                    particleColor = {0x00, 0x00, 0x00, 0xFF};
                    break;

                case PARTICLE_PLANET:
                    particleColor = {0x33, 0x33, 0xFF, 0xFF};
                    break;

                default:
                    particleColor = {0x00, 0x00, 0x00, 0xFF};
            }

            //SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
            setRenderColor(particleColor);
            SDL_RenderFillRect(mRenderer, &fillRect);
        }

        // Draw launcher
        drawLauncher();

        // Commit graphics
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
            } else if (event.key.keysym.sym == SDLK_r) {
                LOGD("%s:\tPressed R -> reset universe\n", __func__);
                mUniverse.reset();
            }
            break;

        case SDL_MOUSEMOTION:
            if (mLauncher.clicked) {
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                mLauncher.end_x = 2*mLauncher.start_x - mx; // Segment length
                mLauncher.end_y = 2*mLauncher.start_y - my; // Segment length
            }
            break;

        case SDL_MOUSEBUTTONDOWN: {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            if (event.button.button == SDL_BUTTON_LEFT) {
                mLauncher.start_x = mx;
                mLauncher.start_y = my;

                mLauncher.end_x = mx;
                mLauncher.end_y = my;
                mLauncher.clicked = true;
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                mUniverse.addBlackHole(mx, my);
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (mLauncher.clicked == false) break;

                int dx = mLauncher.end_x - mLauncher.start_x;
                int dy = mLauncher.end_y - mLauncher.start_y;

                int ex = 1.0 - exp(-abs(dx));
                int ey = 1.0 - exp(-abs(dy));

                mUniverse.addPlanet(
                    mLauncher.start_x,
                    mLauncher.start_y,
                    // Pixels per second
                    ex*dx / FRAMES_PER_SECOND,
                    ey*dy / FRAMES_PER_SECOND);

                mLauncher.clicked = false;
            }

            break;
        }

        default:
            break;
        }
    }
}
