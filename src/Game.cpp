#include <SDL2/SDL.h>
#include "SDL2/SDL2_gfxPrimitives.h"

#include "log.hpp"
#include "gravity.hpp"
#include "Timer.hpp"
#include "Game.hpp"
#include "Universe.hpp"
#include "Launcher.hpp"

Game::Game() :
    mWindow(NULL),
    mRenderer(NULL),
    mUniverse(),
    mTimer()
{
    LOGVV("%s:\tGame constructor\n", __func__);
    mFieldViewSubsambpleX = FIELD_VIEW_SUBSAMPLE_X;
    mFieldViewSubsambpleY = FIELD_VIEW_SUBSAMPLE_Y;

    mGameState.run =  false;
    mGameState.pause = false;
    mGameState.fieldView = false;
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

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOGE("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		ret = -1;
	}
	else
	{
        // Windowed mode by default
        Uint32 windowFlag = SDL_WINDOW_OPENGL;
        bFullScreen = false;

        LOGD("DEFAULT_FULLSCREEN = %d\n", DEFAULT_FULLSCREEN);

        // Set fullscreen if enabled and default
        #if defined ENABLE_FULLSCREEN && DEFAULT_FULLSCREEN == 1
        windowFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
        bFullScreen = true;
        #endif

        mWindow = SDL_CreateWindow (
                wparams->title,             // title
                SDL_WINDOWPOS_UNDEFINED,    // initial x
                SDL_WINDOWPOS_UNDEFINED,    // initial y
                wparams->width,             // width
                wparams->height,            // height
                windowFlag
        );

        if (mWindow == NULL) {
            LOGE("%s:\t Could not create window: %s\n", __func__, SDL_GetError());
            return -1;
        }

        SDL_SetWindowResizable(mWindow, WINDOW_RESIZABLE);


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

    if (state == true) {
        mTimer.pause();
    } else {
        mTimer.unpause();
    }
}

bool Game::togglePause() {
    pause(!mGameState.pause);
}

void Game::eraseOOBParticles() {
    int window_width, window_height;
    SDL_GetWindowSize(mWindow, &window_width, &window_height);

    for (auto p = mUniverse.particles().begin(); p < mUniverse.particles().end(); p++) {
        Particle* particle = *p;
        if (particle->x < -10*window_width || particle->x > 11*window_width ||
            particle->y < -10*window_height || particle->x > 11*window_height)
        {
            mUniverse.particles().erase(p);
            delete particle;
            LOGI("%s:\tErased OOB particle\n", __func__);
        }
    }
}

bool Game::toggleFullScreen() {
    if (!bFullScreen) {
        // Set fullscreen or window
        #if defined ENABLE_FULLSCREEN && ENABLE_FULLSCREEN == 1
        SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        bFullScreen = true;
        #endif
    } else {
        SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_OPENGL);
        bFullScreen = false;
    }

    return bFullScreen;
}

void Game::setRenderColor(Color color) {
    SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
}

void Game::drawLauncher() {
    if (!mLauncher.isClicked()) return;

    // Blue
    Color launcherColor = {
        0xFF - BACKGROUND_COLOR.r,
        0xFF - BACKGROUND_COLOR.g,
        0xFF - BACKGROUND_COLOR.b,
        0xFF,
    };

    setRenderColor(launcherColor);
    SDL_RenderDrawLine(mRenderer,
            mLauncher.getStartX(),
            mLauncher.getStartY(),
            mLauncher.getEndX(),
            mLauncher.getEndY());
}

int Game::run() {
    long frame = 0;
    int window_width, window_height;
    SDL_GetWindowSize(mWindow, &window_width, &window_height);

#if defined(BLACK_HOLE_ON_CREATE) && (BLACK_HOLE_ON_CREATE == 1)
    mUniverse.addStar(window_width/2, window_height/2);
#endif

    // Game loop
    mTimer.start();
    while (mGameState.run == true) {
        // Reset timer and increment frame counter
        mTimer.start();
        frame++;

        /* ======== EVENTS ======== */
        handle_events();

        /* ======== LOGIC ======== */
        if (!mGameState.pause) {
            mUniverse.update();
        }

        /* ======== DRAW ======== */
        // Clear screen
        setRenderColor(BACKGROUND_COLOR);
        SDL_RenderClear(mRenderer);

        // Render the field if enabled
#if defined(ENABLE_FIELD) && (ENABLE_FIELD == 1)
        if (mGameState.fieldView) {
            renderField(mFieldViewSubsambpleX, mFieldViewSubsambpleY);
        }
#endif

        // Draw particles
        Color particleColor;
        for (auto p = mUniverse.particles().begin(); p < mUniverse.particles().end(); p++) {
            Particle* particle = *p;
            // Select particle colour
            particleColor = particle->getColor();

            Color trailColor;
            for (unsigned int i = 1; i < particle->trail.size(); i++) {
                const float frac = static_cast<float>(i) / particle->trail.size();
                particle->getTrailColor(frac, trailColor);
                setRenderColor(trailColor);
                SDL_RenderDrawLine(mRenderer,
                    particle->trail[i].x, particle->trail[i].y,
                    particle->trail[i-1].x, particle->trail[i-1].y);
            }
            if (frame % 2 ==0) {
                particle->updateTrail();
            }

            // Render particle
            setRenderColor(particleColor);
            filledCircleRGBA(mRenderer,
                    particle->x, particle->y, particle->r,
                    particleColor.r,
                    particleColor.g,
                    particleColor.b,
                    particleColor.a);
        }

        // Draw launcher
        drawLauncher();

        // Commit graphics
        SDL_RenderPresent(mRenderer);

        //Sleep until next frame remaining frame time
        int slack = (1000 / FRAMES_PER_SECOND) - mTimer.get_ticks();
        if (slack > 0) {
            int slack = (1000 / FRAMES_PER_SECOND) - mTimer.get_ticks();
            LOGVV("Frame %d, slack %d\n", frame, slack);
            SDL_Delay(slack);
        } else {
            LOGE("Negative slack (%d ms) after frame %d\n", slack, frame);

            // This might help recover some frames
            eraseOOBParticles();
        }
    }

    return 0;
}

#if defined(ENABLE_FIELD) && (ENABLE_FIELD == 1)
void Game::renderField(int subsample_x, int subsample_y) {
    int window_width, window_height;
    SDL_GetWindowSize(mWindow, &window_width, &window_height);

    float max_g = 0.0;
    for (int x = 0; x < window_width; x+=subsample_x) {
        for (int y = 0; y < window_height; y+=subsample_y) {
            float ax = 0.0;
            float ay = 0.0;
            float d, dx, dy, d3;
            for (auto p = mUniverse.particles().begin(); p < mUniverse.particles().end(); p++) {
                Particle* particle = *p;
                dx = (x - particle->x);
                dy = (y - particle->y);
                d = sqrt(dx*dx + dy*dy);
                d3 = d*d*d;
                ax += particle->mass * dx/d3;
                ay += particle->mass * dy/d3;
            }

            float g = sqrt(ax*ax + ay*ay);
            float gth = BLACK_HOLE_MASS / 2000;
            if (g/gth > 1.0) g = gth;

            int aR = 0;   int aG = 0; int aB = 0;  // RGB for our 1st color
            int bR = 128; int bG = 0; int bB= 0;    // RGB for our 2nd color

            Uint8 red   = (bR - aR) * (g/gth) + aR;
            Uint8 green = (bG - aG) * (g/gth) + aG;
            Uint8 blue  = (bB - aB) * (g/gth) + aB;

            Color fieldColor = {red, green, blue, 0xFF};

            setRenderColor(fieldColor);
            if (subsample_x > 1 || subsample_y > 1) {
                SDL_Rect fillRect = {x, y, subsample_x, subsample_y};
                SDL_RenderFillRect(mRenderer, &fillRect);
            } else {
                SDL_RenderDrawPoint(mRenderer, x, y);
            }
        }
    }
}
#endif

void Game::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        LOGVV("%s:\tEvent type = %d\n", __func__, event.type);

        switch(event.type) {
        case SDL_QUIT:
            LOGV("%s:\tSDL_QUIT event\n", __func__);
            mGameState.run = false;
            break;

        case SDL_KEYDOWN:
            LOGD("%s:\tSDL_KEYDOWN event\n", __func__);
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                if (bFullScreen) {
                    LOGD("%s:\tPressed ESCAPE -> exit fullscreen\n", __func__);
                    toggleFullScreen();
                } else {
                    LOGD("%s:\tPressed ESCAPE -> set mGameState.run = false\n", __func__);
                    mGameState.run = false;
                }

            } else if (event.key.keysym.sym == SDLK_p) {
                togglePause();

                LOGD("%s:\tPressed R -> toggle pause to %s\n", __func__,
                    (mGameState.pause==true)?"true":"false");

                const char* title;
                if (mGameState.pause) title = WINDOW_TITLE_PAUSE;
                else title = WINDOW_TITLE;
                SDL_SetWindowTitle(mWindow, title);

            } else if (event.key.keysym.sym == SDLK_r) {
                LOGD("%s:\tPressed P -> reset universe\n", __func__);
                mUniverse.reset();

            } else if (event.key.keysym.sym == SDLK_f) {
                toggleFullScreen();
                LOGD("%s:\tPressed F -> toggle fullscreen %s\n", __func__,
                    bFullScreen? "ON" : "OFF");

#if defined(ENABLE_FIELD) && (ENABLE_FIELD == 1)
            } else if (event.key.keysym.sym == SDLK_g) {
                mGameState.fieldView = !mGameState.fieldView;
                LOGD("%s:\tPressed G -> toggle field view %s\n", __func__,
                        mGameState.fieldView? "ON" : "OFF");
#endif

            } else if (event.key.keysym.sym == SDLK_UP) {
#if defined(ENABLE_FIELD) && (ENABLE_FIELD == 1)
                if (mGameState.fieldView) {
                    mFieldViewSubsambpleY++;
                    LOGD("%s:\tPressed UP -> set field view subsample Y to %d\n", __func__, mFieldViewSubsambpleY);
                }
#endif

            } else if (event.key.keysym.sym == SDLK_DOWN) {
#if defined(ENABLE_FIELD) && (ENABLE_FIELD == 1)
                if (mGameState.fieldView) {
                    if (mFieldViewSubsambpleY > 1) {
                        mFieldViewSubsambpleY--;
                        LOGD("%s:\tPressed UP -> set field view subsample Y to %d\n", __func__, mFieldViewSubsambpleY);
                    }
                }
#endif

            } else if (event.key.keysym.sym == SDLK_LEFT) {
#if defined(ENABLE_FIELD) && (ENABLE_FIELD == 1)
                if (mGameState.fieldView) {
                    if (mFieldViewSubsambpleX > 1) {
                        mFieldViewSubsambpleX--;
                        LOGD("%s:\tPressed UP -> set field view subsample X to %d\n", __func__, mFieldViewSubsambpleX);
                    }
                }
#endif

            } else if (event.key.keysym.sym == SDLK_RIGHT) {
#if defined(ENABLE_FIELD) && (ENABLE_FIELD == 1)
                if (mGameState.fieldView) {
                    mFieldViewSubsambpleX++;
                    LOGD("%s:\tPressed UP -> set field view subsample X to %d\n", __func__, mFieldViewSubsambpleX);
                }
#endif
            }

            break;

        case SDL_MOUSEMOTION:
            if (mLauncher.isClicked()) {
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                mLauncher.move(mx, my);
            }
            break;

        case SDL_MOUSEBUTTONDOWN: {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            if (event.button.button == SDL_BUTTON_LEFT) {
                mLauncher.click(mx, my);
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                mUniverse.addStar(mx, my);
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (!mLauncher.isClicked()) break;
                mLauncher.release();

                const float dx = mLauncher.getStartX() - mLauncher.getEndX();
                const float dy = mLauncher.getStartY() - mLauncher.getEndY();

                mUniverse.addPlanet(
                    mLauncher.getStartX(),
                    mLauncher.getStartY(),
                    dx / FRAMES_PER_SECOND,  // Pixels per second
                    dy / FRAMES_PER_SECOND);
            }

            break;
        }

        default:
            break;
        }
    }
}
