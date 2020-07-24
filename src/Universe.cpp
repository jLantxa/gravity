#include "gravity.hpp"
#include "Universe.hpp"
#include "log.hpp"

#include <cmath>
#include <vector>

Universe::Universe() {
    LOGVV("%s:\tUniverse constructor\n", __func__);
}

Universe::~Universe() {
    LOGVV("%s:\tUniverse destructor\n", __func__);
}

std::vector<Particle>& Universe::particles() {
    return mParticles;
}

void Universe::reset() {
    mParticles.clear();
}

void Universe::addParticle(ParticleType type, bool fixed, float mass, float r, float x, float y, float vx, float vy) {
    Particle particle;
    particle.type = type;
    particle.fixed = fixed;
    particle.mass = mass;
    particle.r = r;
    particle.x = x;
    particle.y = y;
    particle.vx = vx;
    particle.vy = vy;

    mParticles.push_back(particle);

    LOGI("Add particle: %s, mass=%f, x=%f, y=%f, vx=%f, vy=%f\n",
        particle.fixed==true?"fixed":"free",
        particle.mass, particle.x, particle.y, particle.vx, particle.vy);
}

void Universe::addBlackHole(float x, float y) {
    addParticle(PARTICLE_BLACK_HOLE, true, BLACK_HOLE_MASS, BLACK_HOLE_R, x, y, 0, 0);
}

void Universe::addWhiteHole(float x, float y) {
    addParticle(PARTICLE_WHITE_HOLE, true, -BLACK_HOLE_MASS, BLACK_HOLE_R, x, y, 0, 0);
}

void Universe::addPlanet(float x, float y, float vx, float vy) {
    addParticle(PARTICLE_PLANET, false, PLANET_MASS, PLANET_R, x, y, vx, vy);
}

void Universe::update() {
    LOGVV("Updating %d particles\n", mParticles.size());

    constexpr float G = 1.0/FRAMES_PER_SECOND; // Fraction of time [s]*[m/s^2]

    int i = 0;
    int j = 0;

    // Calculate net accellerations
    float aix, aiy; // Instantaneous accelerations
    float d, dx, dy, d3;    // Distance and cubed distance
    bool collision;
    for (auto pi = mParticles.begin(); pi < mParticles.end(); pi++, i++) {
        //if (!pi->fixed)
        {
            for (auto pj = mParticles.begin(); pj < mParticles.end(); pj++, j++) {
                if (pi == pj) continue;

                LOGVV("%s:\tCalculate a%d\n", __func__, i);

                dx = (pj->x - pi->x);
                dy = (pj->y - pi->y);
                d = sqrt(dx*dx + dy*dy);
                d3 = d*d*d;

                #if defined(ENABLE_COLLISIONS) && (ENABLE_COLLISIONS == 1)    // Particles collide with black holes
                if (d < pi->r + pj->r) {
                    LOGI("Particles %d and %d collided\n", i, j);
                    // Bring the two particles together if not fixed
                    collision = true;
                    float new_mass = pi->mass + pj->mass;
                    float new_radius = sqrt(pow(pi->r, 2) + pow(pj->r, 2));
                    auto heavier = (pi->mass >= pj->mass)? pi : pj;
                    auto weaker = (pi->mass < pj->mass)? pi : pj;
                    heavier->vx = (heavier->mass * heavier->vx) + (weaker->mass * weaker->vx);
                    heavier->vx /= new_mass;
                    heavier->vy = (heavier->mass * heavier->vy) + (weaker->mass * weaker->vy);
                    heavier->vy /= new_mass;
                    heavier->mass = new_mass;
                    heavier->r = new_radius;
                    mParticles.erase(weaker);

                    break;
                }
                #endif // ENABLE_COLLISIONS

                aix = pj->mass * dx / d3;
                aiy = pj->mass * dy / d3;

                pi->vx += G * aix;
                pi->vy += G * aiy;

                LOGVV("%s:\ta%d = (%f, %f) [%f]\n", __func__,
                    i, aix, aiy, sqrt(aix*aix + aiy*aiy));
            }
        }
    }

    // Update positions
    i = 0;
    for (auto p = mParticles.begin(); p < mParticles.end(); p++, i++) {
        //if (p->fixed) continue;

        p->x += p->vx;
        p->y += p->vy;

        LOGVV("Update non-fixed particle %d\n", i);
        LOGVV("%s:\tr%d = (%f, %f) [%f]\n", __func__,
            i, p->x, p->y, sqrt(p->x*p->x + p->y*p->y));
        LOGVV("%s:\tv%d = (%f, %f) [%f]\n", __func__,
            i, p->vx, p->vy, sqrt(p->vx*p->vx + p->vy*p->vy));
    }
}
