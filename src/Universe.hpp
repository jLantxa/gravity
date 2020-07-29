#ifndef _UNIVERSE_HPP_
#define _UNIVERSE_HPP_

#define STAR_MASS   10000
#define STAR_R      15
#define PLANET_MASS 1
#define PLANET_R    2

// TODO: Define pixel-to-space transforms
// TODO: Define ticks-to-time transforms

#include <deque>
#include <vector>

constexpr unsigned int MAX_TRAIL_SIZE = 128;

struct Point {
    int x;
    int y;
};

class Particle {
public:
    Particle(float mass, float r, float x, float y, float vx, float vy);
    virtual ~Particle() = default;

    void updateTrail();
    virtual void getTrailColor(float frac, Color& color);
    virtual Color getColor();
    virtual void update();

    float mass;
    float r;
    float x;
    float y;
    float vx;
    float vy;
    std::deque<Point> trail;
};

class Planet : public Particle {
public:
    Planet(float r, float x, float y, float vx, float vy);
    virtual ~Planet() = default;

    void getTrailColor(float frac, Color& color) override;
    Color getColor() override;
};

class Star : public Particle {
public:
    Star(float x, float y);
    virtual ~Star() = default;

    void getTrailColor(float frac, Color& color) override;
    Color getColor() override;

#if defined(FIXED_STARS) && (FIXED_STARS == 1)
    void update() override;
#endif
};


class Universe {
public:
    Universe();
    virtual ~Universe();

    std::vector<Particle*>& particles();

    void update();

    void addParticle(float mass, float r, float x, float y, float vx, float vy);
    void addStar(float x, float y);
    void addPlanet(float x, float y, float vx, float vy);

    void reset();

private:
    std::vector<Particle*> mParticles;
};

#endif
