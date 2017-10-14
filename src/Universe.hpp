#ifndef _UNIVERSE_HPP_
#define _UNIVERSE_HPP_

#define BLACK_HOLE_MASS   50000
#define BLACK_HOLE_R      12
#define PLANET_MASS 50
#define PLANET_R    3

// TODO: Define pixel-to-space transforms
// TODO: Define ticks-to-time transforms

#include <vector>

enum ParticleType {
    PARTICLE_BLACK_HOLE,
    PARTICLE_WHITE_HOLE,
    PARTICLE_PLANET,
    PARTICLE_UNDEFINED,
};

struct Particle {
    ParticleType type;
    bool fixed;
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

    void addParticle(ParticleType type, bool fixed, float mass, float r, float x, float y, float vx, float vy);
    void addBlackHole(float x, float y);
    void addWhiteHole(float x, float y);
    void addPlanet(float x, float y, float vx, float vy);

    void reset();
};

#endif
