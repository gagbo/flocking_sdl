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

#include "entity.h"

std::string Entity::type = "";
std::string Entity::get_type() const { return type; }

Entity::Entity()
    : ent_id(-1), render_window(NULL), parent_world(NULL), size(5, 5),
      position(0, 0), velocity(0, 0), acceleration(0, 0), mass(0),
      max_acceleration(std::numeric_limits<float>::max()), color{0x44, 0x44,
                                                                 0x44, 0xFF},
      friction_factor(0) {}

Entity::Entity(int i, MainWindow &window, World &world)
    : ent_id(i), render_window(&window), parent_world(&world), size(5, 5),
      position(0, 0), velocity(0, 0), acceleration(0, 0), mass(0),
      max_acceleration(std::numeric_limits<float>::max()), color{0x44, 0x44,
                                                                 0x44, 0xFF},
      friction_factor(0) {
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_real_distribution<double> width_dist(0, world.width);
  std::uniform_real_distribution<double> height_dist(0, world.height);

  position(0) = width_dist(e1);
  position(1) = height_dist(e1);
}

Eigen::Vector2d Entity::get_friction_acceleration() {
  Eigen::Vector2d result = -1 * velocity;
  result.normalize();
  float vel_norm = velocity.norm();
  float scaling_factor = vel_norm;

  scaling_factor = friction_factor * vel_norm;

  if (scaling_factor >= mass * vel_norm / parent_world->time_step) {
    scaling_factor = vel_norm * mass / parent_world->time_step;
  }

  result *= scaling_factor / mass;
  return result;
}

void Entity::update() {
  acceleration += get_friction_acceleration();
  velocity += parent_world->time_step * acceleration;
  position += parent_world->time_step * velocity;
  parent_world->wrap_around(position);

  acceleration << 0, 0;
  Eigen::Vector2d screen_pos = parent_world->convert(position);
  Eigen::Vector2d screen_size = parent_world->convert(size);
  render_window->add_FillRect_to_renderer(
      screen_pos(0), screen_pos(1), screen_size(0), screen_size(0), color);
}

void Entity::print() const {
  std::cout << "***********************************\n";
  std::cout << "Position : " << position(0) << "\t" << position(1) << "\n";
  std::cout << "Velocity : " << velocity(0) << "\t" << velocity(1) << "\n";
  std::cout << "Acceleration : " << acceleration(0) << "\t" << acceleration(1)
            << "\n";
}

Entity::~Entity() {}