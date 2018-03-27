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

#ifndef ENTITY_ENTITY_H_
#define ENTITY_ENTITY_H_
#include <Eigen/Dense>
#include <iostream>
#include <limits>
#include <random>

#include "ui/window/mainwindow.h"
#include "world/world.h"

// Entity that lives in World and can move/should be displayed
class Entity {
public:
  // Default constructor
  Entity();
  // Default constructor that points to the window to use to display
  Entity(MainWindow &window);

  ~Entity();

  // update the position according to World::time_step and add shape to
  // renderer for next frame
  void update();

  // Print debug info about the entity
  void print();

  // Accessors
  inline const Eigen::Vector2d& get_pos() { return position; }
  inline const Eigen::Vector2d& get_vel() { return velocity; }

protected:
  // Pointer to the MainWindow on which to draw
  MainWindow *render_window;
  // Size of the bounding rect that represents Entity
  Eigen::Vector2d size;
  // Position of the entity
  Eigen::Vector2d position;
  // Velocity of the entity
  Eigen::Vector2d velocity;
  // Acceleration of the entity
  Eigen::Vector2d acceleration;
  // Mass of the entity
  float mass;
  // Maximum acceleration possible for Entity
  float max_acceleration;
  // Coefficient of friction (drag)
  float friction_factor;
  // Color used to display entity (in hex RGBA)
  int color[4];

  // Compute a linear then quadratic friction acceleration
  Eigen::Vector2d get_friction_acceleration();
};

#endif // ENTITY_ENTITY_H_
