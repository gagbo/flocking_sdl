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

class Entity {
public:
  Entity();
  Entity(MainWindow &window);
  ~Entity();

  void update();
  void print();

protected:
  MainWindow *render_window;
  Eigen::Vector2d position;
  Eigen::Vector2d velocity;
  Eigen::Vector2d acceleration;
  float mass;
  float max_acceleration;
  int color[4];
};

#endif // ENTITY_ENTITY_H_
