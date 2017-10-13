#ifndef _UNIVERSE_HPP_
#define _UNIVERSE_HPP_

#define STAR_MASS   10000
#define STAR_R      20
#define PLANET_MASS 50
#define PLANET_R    5

#include <vector>

struct Particle {
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

    void addParticle(float mass, float r, float x, float y, float vx, float vy);
    void addStar(float x, float y);
    void addPlanet(float x, float y, float vx, float vy);

    void clear();
};

#endif
