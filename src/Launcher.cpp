#include "Launcher.hpp"

Launcher::Launcher() {
    clicked = false;
}

Launcher::~Launcher() {

}

void Launcher::click(int x, int y) {
    clicked = true;
    start_x = x;
    start_y = y;
    end_x = x;
    end_y = y;
}

void Launcher::move(int x, int y) {
    end_x = 2*start_x - x; // Segment length
    end_y = 2*start_y - y; // Segment length
}

void Launcher::release() {
    clicked = false;
}
