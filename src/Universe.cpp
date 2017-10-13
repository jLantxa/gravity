#include "Universe.hpp"
#include "log.hpp"

#include <vector>

Universe::Universe() {
    LOGVV("%s:\tUniverse constructor\n", __func__);
}

Universe::~Universe() {
    LOGVV("%s:\tUniverse destructor\n", __func__);
}

void Universe::addParticle(float mass, float r, float x, float y, float vx, float vy) {
    Particle particle;
    particle.mass = mass;
    particle.r = r;
    particle.x = x;
    particle.y = y;
    particle.vx = vx;
    particle.vy = vy;

    mParticles.push_back(particle);
}

void Universe::addStar(float x, float y) {
    addParticle(STAR_MASS, STAR_R, x, y, 0, 0);
}

void Universe::addPlanet(float x, float y, float vx, float vy) {
    addParticle(PLANET_MASS, PLANET_R, x, y, vx, vy);
}


void Universe::clear() {
    mParticles.clear();
}
