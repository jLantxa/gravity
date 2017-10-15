#ifndef _LAUNCHER_HPP_
#define _LAUNCHER_HPP_

class Launcher {
private:
    bool clicked;
    int start_x;
    int start_y;
    int end_x;
    int end_y;

public:
    Launcher();
    ~Launcher();

    void click(int x, int y);
    void move(int x, int y);
    void release();

    bool isClicked() {return clicked;}
    int getStartX() {return start_x;}
    int getStartY() {return start_y;}
    int getEndX() {return end_x;}
    int getEndY() {return end_y;}
};

#endif
