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

#include "world.h"

int World::width = 320;
int World::height = 240;
float World::time_step = 1.0;

void World::set_world_size(int w, int h) {
  width = w;
  height = h;
}

void World::set_time_step(float t) { time_step = t; }

void World::wrap_around(Eigen::Vector2d &position) {
  while (position(0) >= width || position(0) < 0) {
    if (position(0) < 0) {
      position(0) += width;
    } else if (position(0) >= width) {
      position(0) -= width;
    }
  }
  while (position(1) >= height || position(1) < 0) {
    if (position(1) < 0) {
      position(1) += height;
    } else if (position(1) >= height) {
      position(1) -= height;
    }
  }
}
