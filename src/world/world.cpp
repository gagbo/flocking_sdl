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

#define DEFAULT_WORLD_WIDTH 640
#define DEFAULT_WORLD_HEIGHT 480
#define DEFAULT_PIX_WIDTH 640
#define DEFAULT_PIX_HEIGHT 480
#define DEFAULT_TIME_STEP 1.0

World::World()
    : width(DEFAULT_WORLD_WIDTH), height(DEFAULT_WORLD_HEIGHT),
      time_step(DEFAULT_TIME_STEP) {}

World::World(int w, int h, float dt) : width(w), height(h), time_step(dt) {}

World::~World() {}

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

Eigen::Vector2d World::convert(const Eigen::Vector2d &position) {
  Eigen::Matrix2d transform;
  transform << static_cast<float>(width_in_px) / width, 0, 0,
      static_cast<float>(height_in_px) / height;
  Eigen::Vector2d result = transform * position;
  result(0) = std::floor(result(0));
  result(1) = std::floor(result(1));
  return result;
}

Eigen::Vector2d World::point_to(const Eigen::Vector2d &tail,
                                const Eigen::Vector2d &head) {
  if (!(head(0) >= 0 && head(1) < width && tail(0) >= 0 && tail(1) < height)) {
    throw std::runtime_error(
        "The provided vectors for point_to are not within World square !");
  }
  Eigen::Vector2d result = head - tail;

  if (result(0) > width / 2) {
    result(0) -= width;
  } else if (result(0) < -width / 2) {
    result(0) += width;
  }

  if (result(1) > height / 2) {
    result(1) -= height;
  } else if (result(1) < -height / 2) {
    result(1) += height;
  }

  return result;
}
