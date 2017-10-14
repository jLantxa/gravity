#include <SDL2/SDL.h>
#include "SDL2/SDL2_gfxPrimitives.h"

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
    mFieldViewSubsambpleX = FIELD_VIEW_SUBSAMPLE_X;
    mFieldViewSubsambpleY = FIELD_VIEW_SUBSAMPLE_Y;
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
}

void Game::eraseOOBParticles() {
    int window_width, window_height;
    SDL_GetWindowSize(mWindow, &window_width, &window_height);

    for (auto p = mUniverse.particles().begin(); p < mUniverse.particles().end(); p++) {
        if (p->x < -10*window_width || p->x > 11*window_width ||
            p->y < -10*window_height || p->x > 11*window_height)
        {
            mUniverse.particles().erase(p);
            LOGI("%s:\tErased OOB particle\n", __func__);
        }
    }
}

bool Game::toggleFullScreen() {
    if (!bFullScreen) {
        // Set fullscreen or window
        #if defined ENABLE_FULLSCREEN
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
        if (!mGameState.pause) {
            mUniverse.update();
        }

        /* ======== DRAW ======== */
        // Clear screen
        setRenderColor(BACKGROUND_COLOR);
        SDL_RenderClear(mRenderer);

        // Rendef the field if enabled
        if (bFieldView) {
            renderField(mFieldViewSubsambpleX, mFieldViewSubsambpleY);
        }

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

                case PARTICLE_WHITE_HOLE:
                    particleColor = {0xFF, 0xFF, 0xAA, 0xFF};
                    break;

                case PARTICLE_PLANET:
                    if (bFieldView) particleColor = {0xFF, 0x33, 0x33, 0xFF};
                    else particleColor = {0x33, 0x33, 0xFF, 0xFF};
                    break;

                default:
                    particleColor = {0x128, 0x128, 0x128, 0xFF};
            }

            setRenderColor(particleColor);
            filledCircleRGBA(mRenderer,
                    p->x, p->y, p->r,
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
        int slack = (1000 / FRAMES_PER_SECOND) - fps.get_ticks();
        if (slack > 0) {
            int slack = (1000 / FRAMES_PER_SECOND) - fps.get_ticks();
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
                dx = (x - p->x);
                dy = (y - p->y);
                d = sqrt(dx*dx + dy*dy);
                d3 = d*d*d;
                ax += p->mass * dx/d3;
                ay += p->mass * dy/d3;
            }

            float g = sqrt(ax*ax + ay*ay);
            float gth = BLACK_HOLE_MASS / 2000;
            if (g/gth > 1.0) g = gth;

            int aR = 0;   int aG = 0; int aB = 128;  // RGB for our 1st color
            int bR = 128; int bG = 128; int bB=0;    // RGB for our 2nd color

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
            } else if (event.key.keysym.sym == SDLK_p) {
                mGameState.pause = !mGameState.pause;
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
            } else if (event.key.keysym.sym == SDLK_g) {
                bFieldView = !bFieldView;
                LOGD("%s:\tPressed G -> toggle field view %s\n", __func__,
                        bFieldView? "ON" : "OFF");
            } else if (event.key.keysym.sym == SDLK_a) {
                bAntiMatter = true;
                LOGD("%s:\tPressed A -> enable anti-matter%s\n", __func__);
            }

            else if (event.key.keysym.sym == SDLK_UP) {
                if (bFieldView) {
                    mFieldViewSubsambpleY++;
                    LOGD("%s:\tPressed UP -> set field view subsample Y to %d\n", __func__, mFieldViewSubsambpleY);
                }
            } else if (event.key.keysym.sym == SDLK_DOWN) {
                if (bFieldView) {
                    if (mFieldViewSubsambpleY > 1) {
                        mFieldViewSubsambpleY--;
                        LOGD("%s:\tPressed UP -> set field view subsample Y to %d\n", __func__, mFieldViewSubsambpleY);
                    }
                }
            } else if (event.key.keysym.sym == SDLK_LEFT) {
                if (bFieldView) {
                    if (mFieldViewSubsambpleX > 1) {
                        mFieldViewSubsambpleX--;
                        LOGD("%s:\tPressed UP -> set field view subsample X to %d\n", __func__, mFieldViewSubsambpleX);
                    }
                }
            } else if (event.key.keysym.sym == SDLK_RIGHT) {
                if (bFieldView) {
                    mFieldViewSubsambpleX++;
                    LOGD("%s:\tPressed UP -> set field view subsample X to %d\n", __func__, mFieldViewSubsambpleX);
                }
            }

            break;

        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_a) {
                bAntiMatter = false;
                LOGD("%s:\tReleased A -> disable anti-matter%s\n", __func__);
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
                if (!bAntiMatter) mUniverse.addBlackHole(mx, my);
                else mUniverse.addWhiteHole(mx, my);
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
