#include "log.hpp"

#include "gravity.hpp"
#include "Game.hpp"

int main(int argc, char* argv[])
{
    int ret = 0;
    Game game;

    // Fill window params
    window_params wparams {
        .title  = WINDOW_TITLE,
        .width  = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT
    };

    ret = game.init(&wparams);
    if (ret != 0) {
        return ret;
    }

    LOGV("Running game...\n");
    ret = game.run();

    // End of loop

    LOGD("Exit with return code %d\n", ret);
    return ret;
}
