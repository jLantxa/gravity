#ifndef _UNIVERSE_HPP_
#define _UNIVERSE_HPP_

#define BLACK_HOLE_MASS   10000
#define BLACK_HOLE_R      15
#define PLANET_MASS 1
#define PLANET_R    2

// TODO: Define pixel-to-space transforms
// TODO: Define ticks-to-time transforms

#include <vector>

enum ParticleType {
    PARTICLE_STAR,
    PARTICLE_PLANET,
    PARTICLE_UNDEFINED,
};

struct Particle {
    ParticleType type;
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

    std::vector<Particle>& particles();

    void update();

    void addParticle(ParticleType type, float mass, float r, float x, float y, float vx, float vy);
    void addStar(float x, float y);
    void addPlanet(float x, float y, float vx, float vy);

    void reset();
};

#endif
