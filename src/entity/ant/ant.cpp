/* Copyright (c) 2018 Gerry Agbobada
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ant.h"

Ant::Ant() : Entity() {
  type = Entity::Type::ANT;
  mass = 1;
  friction_factor = 1e-2;
}

Ant::Ant(int i, World &parent_world) : Entity(i, parent_world) {
  type = Entity::Type::ANT;
  mass = 1;
  friction_factor = 1e-2;
}

void Ant::decision() {
  acceleration << 0.5, -1;
  cap_force(1e-3);
}

void Ant::cap_acceleration() {
  float norm = acceleration.norm();
  if (norm > max_acceleration) {
    acceleration.normalize();
    acceleration = max_acceleration * acceleration;
  }
}

void Ant::cap_force(float max_force) {
  max_acceleration = max_force / mass;
  cap_acceleration();
}

Ant::~Ant() {}
