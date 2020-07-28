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
    mParticles.clear();
}

std::vector<Particle*>& Universe::particles() {
    return mParticles;
}

Particle::Particle(float mass, float r, float x, float y, float vx, float vy)
:   mass(mass),
    r(r),
    x(x),
    y(y),
    vx(vx),
    vy(vy)
{

}

Color Particle::getColor() {
    constexpr Color color{0xFF, 0xFF, 0xFF, 0xFF};
    return color;
}

Planet::Planet(float r, float x, float y, float vx, float vy)
:   Particle(PLANET_MASS, r, x, y, vx, vy)
{

}

Color Planet::getColor() {
    constexpr Color color{0xFF, 0xFF, 0xFF, 0xFF};
    return color;
}

Star::Star(float x, float y)
:   Particle(BLACK_HOLE_MASS, BLACK_HOLE_R, x, y, 0, 0)
{

}

Color Star::getColor() {
    constexpr Color color{0xFF, 0x3F, 0x00, 0xFF};
    return color;
}

void Particle::updateTrail() {
    if (trail.size() >= MAX_TRAIL_SIZE) {
        trail.pop_front();
    }
    trail.push_back(Point{static_cast<int>(x), static_cast<int>(y)});
}

void Particle::getTrailColor(float frac, Color& color) {
    color.r = 0xFF * frac;
    color.g = 0xFF * frac;
    color.b = 0xFF * frac;
    color.a = 0xFF * (1 - frac);
}

void Planet::getTrailColor(float frac, Color& color) {
    color.r = 0x00;
    color.g = 0x00;
    color.b = 0xFF * frac;
    color.a = 0xFF * (1 - frac);
}

void Star::getTrailColor(float frac, Color& color) {
    color.r = 0xFF * frac;
    color.g = 0x00;
    color.b = 0x00;
    color.a = 0xFF * (1 - frac);
}

void Universe::reset() {
    mParticles.clear();
}

void Universe::addParticle(float mass, float r, float x, float y, float vx, float vy) {
    Particle* particle = new Particle(mass, r, x, y, vx, vy);
    mParticles.push_back(particle);

    LOGI("Add star: mass=%f, x=%f, y=%f, vx=%f, vy=%f\n",
        particle->mass, particle->x, particle->y, particle->vx, particle->vy);
}

void Universe::addStar(float x, float y) {
    Star* star = new Star(x, y);
    mParticles.push_back(star);
    LOGI("Add star: mass=%f, x=%f, y=%f\n",
        star->mass, star->x, star->y);
}

void Universe::addPlanet(float x, float y, float vx, float vy) {
    Planet* planet = new Planet(PLANET_R, x, y, vx, vy);
    mParticles.push_back(planet);
    LOGI("Add planet: mass=%f, x=%f, y=%f, vx=%f, vy=%f\n",
        planet->mass, planet->x, planet->y, planet->vx, planet->vy);
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
        for (auto pj = mParticles.begin(); pj < mParticles.end(); pj++, j++) {
            if (pi == pj) continue;

            Particle* part_i = *pi;
            Particle* part_j = *pj;

            LOGVV("%s:\tCalculate a%d\n", __func__, i);

            dx = (part_j->x - part_i->x);
            dy = (part_j->y - part_i->y);
            d = sqrt(dx*dx + dy*dy);
            d3 = d*d*d;

            #if defined(ENABLE_COLLISIONS) && (ENABLE_COLLISIONS == 1)    // Particles collide with black holes
            if (d < part_i->r + part_j->r) {
                LOGI("Particles %d and %d collided\n", i, j);
                // Bring the two particles together
                collision = true;
                float new_mass = part_i->mass + part_j->mass;
                float new_radius = sqrt(pow(part_i->r, 2) + pow(part_j->r, 2));
                auto heavier = (part_i->mass >= part_j->mass)? part_i : part_j;
                auto weaker_it = (part_i->mass < part_j->mass)? pi : pj;
                auto weaker = *weaker_it;
                heavier->vx = (heavier->mass * heavier->vx) + (weaker->mass * weaker->vx);
                heavier->vx /= new_mass;
                heavier->vy = (heavier->mass * heavier->vy) + (weaker->mass * weaker->vy);
                heavier->vy /= new_mass;
                heavier->mass = new_mass;
                heavier->r = new_radius;
                mParticles.erase(weaker_it);

                break;
            }
            #endif // ENABLE_COLLISIONS

            aix = part_j->mass * dx / d3;
            aiy = part_j->mass * dy / d3;

            part_i->vx += G * aix;
            part_i->vy += G * aiy;

            LOGVV("%s:\ta%d = (%f, %f) [%f]\n", __func__,
                i, aix, aiy, sqrt(aix*aix + aiy*aiy));
        }
    }

    // Update positions
    i = 0;
    for (auto p = mParticles.begin(); p < mParticles.end(); p++, i++) {
        Particle* particle = *p;
        particle->x += particle->vx;
        particle->y += particle->vy;

        LOGVV("%s:\tr%d = (%f, %f) [%f]\n", __func__,
            i, particle->x, particle->y, sqrt(particle->x*particle->x + particle->y*particle->y));
        LOGVV("%s:\tv%d = (%f, %f) [%f]\n", __func__,
            i, particle->vx, particle->vy, sqrt(particle->vx*particle->vx + particle->vy*particle->vy));
    }
}
