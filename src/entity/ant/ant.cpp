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

Ant::Ant()
    : render_window(NULL), position(0, 0), velocity(0, 0), mass(0),
      max_acceleration(std::numeric_limits<float>::max()) {}

Ant::Ant(MainWindow &window)
    : render_window(&window), position(0, 0), velocity(0, 0), mass(0),
      max_acceleration(std::numeric_limits<float>::max()) {
  position = Eigen::Vector2d::Random();
  position(0) = (position(0) + 1.0) * World::width / 2;
  position(1) = (position(1) + 1.0) * World::height / 2;
}

void Ant::decision() {}

void Ant::update() {}

Ant::~Ant() {}
