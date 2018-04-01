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

// Since World will instantiate all makeEntity templates, we need fully defined
// Entity Derived Classes
#include "entity/ant/ant.h"
#include "entity/food/food.h"
#include "ui/window/mainwindow.h"
#include "world.h"

#define DEFAULT_WORLD_WIDTH 640
#define DEFAULT_WORLD_HEIGHT 480
#define DEFAULT_PIX_WIDTH 640
#define DEFAULT_PIX_HEIGHT 480
#define DEFAULT_TIME_STEP 1.0

World::World()
    : width(DEFAULT_WORLD_WIDTH),
      height(DEFAULT_WORLD_HEIGHT),
      time_step(DEFAULT_TIME_STEP),
      entity_list(),
      entity_count(),
      render_window(NULL) {}

World::World(int w, int h, float dt)
    : width(w),
      height(h),
      time_step(dt),
      entity_list(),
      entity_count(),
      render_window(NULL) {}

World::~World() {}

void World::set_render_window(MainWindow &window) { render_window = &window; }

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

Eigen::Vector2d World::convert(const Eigen::Vector2d &position) const {
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
    if (!(head(0) >= 0 && head(1) < width && tail(0) >= 0 &&
          tail(1) < height)) {
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

MainWindow &World::get_mut_window() { return *render_window; }

void World::update() {
    for (auto &&entity : entity_list) {
        entity->decision();
        entity->update();
        Eigen::Vector2d screen_pos = convert(entity->get_pos());
        Eigen::Vector2d screen_size = convert(entity->get_size());
        render_window->add_FillRect_to_renderer(screen_pos(0), screen_pos(1),
                                                screen_size(0), screen_size(0),
                                                entity->get_color());
    }
}

void World::add_entity(Entity::Type type, float x, float y) {
    int next_id;
    try {
        next_id = entity_count.at(type);
    } catch (const std::out_of_range &e) {
        entity_count[type] = 0;
        next_id = 0;
    }

    if (x < 0 || y < 0) {
        auto p_newEnt = Entity::makeEntity(type, next_id, *this);
        entity_list.push_back(std::move(p_newEnt));
    } else {
        auto p_newEnt = Entity::makeEntity(type, next_id, *this, x, y);
        entity_list.push_back(std::move(p_newEnt));
    }
    entity_count[type]++;
}
