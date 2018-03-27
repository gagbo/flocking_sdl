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

#ifndef WORLD_WORLD_H_
#define WORLD_WORLD_H_
#include <Eigen/Dense>
#include <iostream>
#include <stdexcept>

// A public struct that contains world-related helpers/definitions
class World {
public:
  World();
  World(int w, int h, float dt);
  ~World();

  // Width of the world in arbitrary unit
  int width;
  // Height of the world in arbitrary unit
  int height;
  // Width of the world in pixels
  int width_in_px;
  // Height of the world in pixels
  int height_in_px;
  // Time step for the physics engine
  float time_step;

  // Setter for the world size
  void set_world_size(int w, int h);
  // Setter for the time step
  void set_time_step(float t);
  // Translate position in place so the world wraps around edges
  void wrap_around(Eigen::Vector2d &position);
  // Translate arbitrary unit to pixels position for the renderer
  Eigen::Vector2d convert(const Eigen::Vector2d &position);
  // Computes the vector to go from tail to head
  // This only work on 'wrapped_around' vectors, for which coord
  //   lie in [0 ; width] x [0 ; height]
  Eigen::Vector2d point_to(const Eigen::Vector2d &tail,
                           const Eigen::Vector2d &head);
};

#endif // WORLD_WORLD_H_
