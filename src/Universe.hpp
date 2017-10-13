#ifndef _UNIVERSE_HPP_
#define _UNIVERSE_HPP_

#define STAR_MASS   10000
#define STAR_R      20
#define PLANET_MASS 50
#define PLANET_R    5

// TODO: Define pixel-to-space transforms
// TODO: Define ticks-to-time transforms

#include <vector>

struct Particle {
    bool fixed;
    bool falling;
    float mass;
    float r;
    float x;
    float y;
    float vx;
    float vy;
};

class Universe {
private:
    std::vector<Particle> mParticles;

public:
    Universe();
    virtual ~Universe();

    void update();

    void addParticle(bool fixed, float mass, float r, float x, float y, float vx, float vy);
    void addStar(float x, float y);
    void addPlanet(float x, float y, float vx, float vy);

    void clear();
};

#endif
