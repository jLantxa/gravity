#include "SDL2/SDL.h"

#include "log.hpp"
#include "gravity.hpp"
#include "Game.hpp"

struct window_params {
    char* title;
    int width;
    int height;
};

int main(int argc, char* argv[])
{
    int ret = 0;

    // Fill window params
    window_params wparams;
    wparams.title = WINDOW_TITLE;
    wparams.width = WINDOW_WIDTH;
    wparams.height = WINDOW_HEIGHT;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* gWindow;
    SDL_Surface* gScreenSurface;

    LOGI("Create window (%dx%d)\n", wparams.width, wparams.height);
    gWindow = SDL_CreateWindow (
            wparams.title,              // title
            SDL_WINDOWPOS_UNDEFINED,    // initial x
            SDL_WINDOWPOS_UNDEFINED,    // initial y
            wparams.width,              // width
            wparams.height,             // height
            SDL_WINDOW_OPENGL
    );

    if (gWindow == NULL) {
        LOGE("%s:\t Could not create window: %s\n", __func__, SDL_GetError());
        return -1;
    }

    Game game(gScreenSurface);

    LOGV("Running game...\n");
    ret = game.run();

    // End of loop
    LOGV("Freeing screen surface...\n");
    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;
    LOGV("Destroying window... \n", __FUNCTION__);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    LOGD("Exit with return code %d\n", ret);
    return ret;
}
