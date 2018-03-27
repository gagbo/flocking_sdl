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

Entity::Entity()
    : render_window(NULL), size(5,5), position(0, 0), velocity(0, 0), acceleration(0, 0),
      mass(0),
      max_acceleration(std::numeric_limits<float>::max()), color{0x44, 0x44,
                                                                 0x44, 0xFF} {}

Entity::Entity(MainWindow &window)
    : render_window(&window), size(5,5), position(0, 0), velocity(0.5, 0.25),
      acceleration(0, 0), mass(0),
      max_acceleration(std::numeric_limits<float>::max()), color{0x44, 0x44,
                                                                 0x44, 0xFF} {
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_real_distribution<double> width_dist(0, World::width);
  std::uniform_real_distribution<double> height_dist(0, World::height);

  position(0) = width_dist(e1);
  position(1) = height_dist(e1);
}

void Entity::update() {
  velocity += World::time_step * acceleration;
  position += World::time_step * velocity;
  World::wrap_around(position);

  acceleration = Eigen::Vector2d(0, 0);
  Eigen::Vector2d screen_pos = World::convert(position);
  Eigen::Vector2d screen_size = World::convert(size);
  render_window->add_FillRect_to_renderer(screen_pos(0), screen_pos(1), screen_size(0),
                                          screen_size(0), color);
}

void Entity::print() {
  std::cout << "***********************************\n";
  std::cout << "Position : " << position(0) << "\t" << position(1) << "\n";
  std::cout << "Velocity : " << velocity(0) << "\t" << velocity(1) << "\n";
  std::cout << "Acceleration : " << acceleration(0) << "\t" << acceleration(1)
            << "\n";
}

Entity::~Entity() {}
