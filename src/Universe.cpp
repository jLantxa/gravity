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

std::vector<Particle> Universe::particles() {
    return mParticles;
}

void Universe::addParticle(bool fixed, float mass, float r, float x, float y, float vx, float vy) {
    Particle particle;
    particle.fixed = fixed;
    particle.falling =  true;
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

void Universe::addStar(float x, float y) {
    addParticle(true, STAR_MASS, STAR_R, x, y, 0, 0);
}

void Universe::addPlanet(float x, float y, float vx, float vy) {
    addParticle(false, PLANET_MASS, PLANET_R, x, y, vx, vy);
}

void Universe::clear() {
    mParticles.clear();
}

void Universe::update() {
    LOGVV("Updating %d particles\n", mParticles.size());
    float k = 1.0; // Fraction of time [s]*[m/s^2]

    int i = 0;
    int j = 0;

    // Calculate net accellerations
    float aix, aiy; // Instantaneous accelerations
    float d, d3;    // Distance and cubed distance
    for (auto pi = mParticles.begin(); pi < mParticles.end(); pi++, i++) {
        if (!pi->fixed)
        {
            for (auto pj = mParticles.begin(); pj < mParticles.end(); pj++, j++) {
                if (pi == pj) continue;

                LOGVV("%s:\tCalculate a%d\n", __func__, i);

                d = sqrt((pi->x - pj->x)*(pi->x - pj->x) + (pi->y - pj->y)*(pi->y - pj->y));

                // Particles collide
                if (d < pi->r + pj->r) {
                    // Destroy particle if not fixed
                    if (!pi->fixed) {
                        mParticles.erase(pi);
                        LOGI("Particle %d was destroyed\n", i);
                    }

                    if (!pj->fixed) {
                        mParticles.erase(pj);
                        LOGI("Particle %d was destroyed\n", j);
                    }

                    continue;
                }

                d3 = pow(d, 3);
                aix = pj->mass * (pj->x - pi->x) / d3;
                aiy = pj->mass * (pj->y - pi->y) / d3;

                pi->vx += k * aix;
                pi->vy += k * aiy;

                LOGVV("%s:\ta%d = (%f, %f)", __func__,
                    i, aix, aiy);
            }
        }
    }

    // Update positions
    i = 0;
    for (auto p = mParticles.begin(); p < mParticles.end(); p++, i++) {
        if (p->fixed) continue;

        p->x += p->vx;
        p->y += p->vy;

        LOGVV("Update non-fixed particle %d\n", i);
        LOGVV("%s:\tr%d = (%f, %f)", __func__,
            i, p->x, p->y);
    }
}
