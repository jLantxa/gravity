/*
 * gravity
 * Copyright (C) 2017 - 2020  Javier Lancha Vázquez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
