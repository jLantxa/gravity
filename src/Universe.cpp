/*
 * gravity
 * Copyright (C) 2017 - 2024  Javier Lancha Vázquez
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

#include "Universe.hpp"
#include "gravity.hpp"
#include "log.hpp"

#include <cmath>
#include <vector>

Universe::Universe() { LOGVV("%s:\tUniverse constructor\n", __func__); }

Universe::~Universe() {
  LOGVV("%s:\tUniverse destructor\n", __func__);
  mParticles.clear();
}

std::vector<Particle *> &Universe::particles() { return mParticles; }

Particle::Particle(float mass, float r, float x, float y, float vx, float vy)
    : mass(mass), r(r), x(x), y(y), vx(vx), vy(vy) {}

Color Particle::getColor() {
  constexpr Color color{0xFF, 0xFF, 0xFF, 0xFF};
  return color;
}

Planet::Planet(float r, float x, float y, float vx, float vy)
    : Particle(PLANET_MASS, r, x, y, vx, vy) {}

Color Planet::getColor() {
  constexpr Color color{0xFF, 0xFF, 0xFF, 0xFF};
  return color;
}

Star::Star(float x, float y) : Particle(STAR_MASS, STAR_R, x, y, 0, 0) {}

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

void Particle::getTrailColor(float frac, Color &color) {
  color.r = 0xFF * frac;
  color.g = 0xFF * frac;
  color.b = 0xFF * frac;
  color.a = 0xFF * (1 - frac);
}

void Particle::update() {
  x += vx;
  y += vy;
}

void Planet::getTrailColor(float frac, Color &color) {
  color.r = 0x00;
  color.g = 0x00;
  color.b = 0xFF * frac;
  color.a = 0xFF * (1 - frac);
}

void Star::getTrailColor(float frac, Color &color) {
  color.r = 0xFF * frac;
  color.g = 0x00;
  color.b = 0x00;
  color.a = 0xFF * (1 - frac);
}

#if defined(FIXED_STARS) && (FIXED_STARS == 1)
void Star::update() {
  vx = 0;
  vy = 0;
}
#endif

void Universe::reset() { mParticles.clear(); }

void Universe::addParticle(float mass, float r, float x, float y, float vx,
                           float vy) {
  Particle *particle = new Particle(mass, r, x, y, vx, vy);
  mParticles.push_back(particle);

  LOGI("Add star: mass=%f, x=%f, y=%f, vx=%f, vy=%f\n", particle->mass,
       particle->x, particle->y, particle->vx, particle->vy);
}

void Universe::addStar(float x, float y) {
  Star *star = new Star(x, y);
  mParticles.push_back(star);
  LOGI("Add star: mass=%f, x=%f, y=%f\n", star->mass, star->x, star->y);
}

void Universe::addPlanet(float x, float y, float vx, float vy) {
  Planet *planet = new Planet(PLANET_R, x, y, vx, vy);
  mParticles.push_back(planet);
  LOGI("Add planet: mass=%f, x=%f, y=%f, vx=%f, vy=%f\n", planet->mass,
       planet->x, planet->y, planet->vx, planet->vy);
}

void Universe::update() {
  LOGVV("Updating %d particles\n", mParticles.size());

  constexpr float G =
      1.0 / constants::FRAMES_PER_SECOND; // Fraction of time [s]*[m/s^2]

  int i = 0;
  int j = 0;

  // Calculate net accellerations
  float aix, aiy;      // Instantaneous accelerations
  float d, dx, dy, d3; // Distance and cubed distance
  for (auto pi = mParticles.begin(); pi < mParticles.end(); pi++, i++) {
    for (auto pj = mParticles.begin(); pj < mParticles.end(); pj++, j++) {
      if (pi == pj)
        continue;

      Particle *part_i = *pi;
      Particle *part_j = *pj;

      LOGVV("%s:\tCalculate a%d\n", __func__, i);

      dx = (part_j->x - part_i->x);
      dy = (part_j->y - part_i->y);
      d = sqrt(dx * dx + dy * dy);
      d3 = d * d * d;

      if (d < part_i->r + part_j->r) {
        LOGI("Particles %d and %d collided\n", i, j);
        // Bring the two particles together
        float new_mass = part_i->mass + part_j->mass;
        float new_radius = sqrt(pow(part_i->r, 2) + pow(part_j->r, 2));
        auto heavier = (part_i->mass >= part_j->mass) ? part_i : part_j;
        auto weaker_it = (part_i->mass < part_j->mass) ? pi : pj;
        auto weaker = *weaker_it;
        heavier->vx =
            (heavier->mass * heavier->vx) + (weaker->mass * weaker->vx);
        heavier->vx /= new_mass;
        heavier->vy =
            (heavier->mass * heavier->vy) + (weaker->mass * weaker->vy);
        heavier->vy /= new_mass;
        heavier->mass = new_mass;
        heavier->r = new_radius;
        mParticles.erase(weaker_it);
        break;
      }

      aix = part_j->mass * dx / d3;
      aiy = part_j->mass * dy / d3;

      part_i->vx += G * aix;
      part_i->vy += G * aiy;

      LOGVV("%s:\ta%d = (%f, %f) [%f]\n", __func__, i, aix, aiy,
            sqrt(aix * aix + aiy * aiy));
    }
  }

  // Update positions
  for (Particle *particle : mParticles) {
    particle->update();
  }
}
