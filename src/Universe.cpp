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

void Universe::addParticle(bool fixed, float mass, float r, float x, float y, float vx, float vy) {
    Particle particle;
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

    float k = 0.1*1.0/FRAMES_PER_SECOND; // Fraction of time [s]*[m/s^2]

    int i = 0;
    int j = 0;

    // Calculate net accellerations
    float aix, aiy; // Instantaneous accelerations
    float d, d3;    // Distance and cubed distance
    bool collision;
    for (auto pi = mParticles.begin(); pi < mParticles.end(); pi++, i++) {
        collision = false;
        if (!pi->fixed)
        {
            for (auto pj = mParticles.begin(); pj < mParticles.end(); pj++, j++) {
                if (pi == pj) continue;

                LOGVV("%s:\tCalculate a%d\n", __func__, i);

                d = sqrt((pi->x - pj->x)*(pi->x - pj->x) + (pi->y - pj->y)*(pi->y - pj->y));

                // Particles collide
                if (d < pi->r + pj->r) {
                    LOGI("Particles %d and %d collided\n", i, j);
                    // Brint the two particles together if not fixed
                    if (!pi->fixed) {
                        if (!pj->fixed) {
                            pi->mass += pj->mass;
                            pi->r *= sqrt(2);
                            pi->vx += pj->vx;
                            pi->vy += pj->vy;
                            pi->x = (pi->x + pj->x) / 2;
                            pi->y = (pi->y + pj->y) / 2;
                            mParticles.erase(pj);
                        } else {
                            collision = true;
                        }
                    }
                    continue;
                }

                d3 = pow(d, 2);
                aix = pj->mass * (pj->x - pi->x) / d3;
                aiy = pj->mass * (pj->y - pi->y) / d3;

                pi->vx += k * aix;
                pi->vy += k * aiy;

                LOGVV("%s:\ta%d = (%f, %f) [%f]\n", __func__,
                    i, aix, aiy, sqrt(aix*aix + aiy*aiy));
            }

            if (collision) {
                mParticles.erase(pi);
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
        LOGVV("%s:\tr%d = (%f, %f) [%f]\n", __func__,
            i, p->x, p->y, sqrt(p->x*p->x + p->y*p->y));
        LOGVV("%s:\tv%d = (%f, %f) [%f]\n", __func__,
            i, p->vx, p->vy, sqrt(p->vx*p->vx + p->vy*p->vy));
    }
}
