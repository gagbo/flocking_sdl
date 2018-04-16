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
#include <fstream>
#include "FlockingConfig.h"
#include "entity/ant/ant.h"
#include "entity/food/food.h"
#include "jsoncpp/json/json.h"
#include "ui/window/mainwindow.h"
#include "world.h"

World::World(int w, int h, float dt) : _width(w), _height(h), _time_step(dt) {}

World::~World() {}

void World::set_render_window(MainWindow &window) { _render_window = &window; }

void World::set_world_size(int w, int h) {
    _width = w;
    _height = h;
}

void World::set_time_step(float t) { _time_step = t; }

void World::wrap_around(Eigen::Vector2d &position) {
    while (position(0) >= _width || position(0) < 0) {
        if (position(0) < 0) {
            position(0) += _width;
        } else if (position(0) >= _width) {
            position(0) -= _width;
        }
    }
    while (position(1) >= _height || position(1) < 0) {
        if (position(1) < 0) {
            position(1) += _height;
        } else if (position(1) >= _height) {
            position(1) -= _height;
        }
    }
}

Eigen::Vector2d World::convert(const Eigen::Vector2d &position) const {
    Eigen::Matrix2d transform;
    transform << static_cast<float>(_width_in_px) / _width, 0, 0,
        static_cast<float>(_height_in_px) / _height;
    Eigen::Vector2d result = transform * position;
    result(0) = std::floor(result(0));
    result(1) = std::floor(result(1));
    return result;
}

Eigen::Vector2d World::point_to(const Eigen::Vector2d &tail,
                                const Eigen::Vector2d &head) {
    if (!(head(0) >= 0 && head(1) < _width && tail(0) >= 0 &&
          tail(1) < _height)) {
        throw std::runtime_error(
            "The provided vectors for point_to are not within World square !");
    }
    Eigen::Vector2d result = head - tail;

    if (result(0) > _width / 2) {
        result(0) -= _width;
    } else if (result(0) < -_width / 2) {
        result(0) += _width;
    }

    if (result(1) > _height / 2) {
        result(1) -= _height;
    } else if (result(1) < -_height / 2) {
        result(1) += _height;
    }

    return result;
}

MainWindow &World::get_mut_window() { return *_render_window; }

void World::update() {
    _time += _time_step;
    find_and_serve_new_events();
    update_tree();
    update_entity_neighbourhoods();
    call_entity_decision();
    update_entity_and_renderer();
}

void World::find_and_serve_new_events() {
    auto new_events_to_serve =
        _events.events_in_time_frame(_time - _time_step, _time);

    for (auto &&event : new_events_to_serve) {
        serve_json_event(event);
    }
}

void World::update_tree() {
    _entity_tree.clean();
    for (auto &&entity : _entity_list) {
        _entity_tree.insert(entity);
    }
}

void World::update_entity_neighbourhoods() {
    for (auto &&entity : _entity_list) {
        entity->clear_neighbours();
        // Base case, get all neighbours inside
        auto ent_neighbours = _entity_tree.norm1_range_query(
            *entity, entity->vision_distance());
        // Add the neighbours from the edges in case of wrapping around
        float x = entity->pos()(0);
        float y = entity->pos()(1);
        float radius = entity->vision_distance();
        if (x - radius < 0) {
            Eigen::Vector2d fictive_position(x + _width, y);
            float fictive_radius(radius - x);
            auto add_neighbours =
                _entity_tree.norm1_range_query(fictive_position, fictive_radius);
            ent_neighbours.insert(ent_neighbours.end(), add_neighbours.begin(),
                                  add_neighbours.end());
            if (y - radius < 0) {
                Eigen::Vector2d fictive_position2(x + _width, y + _height);
                float fictive_radius2(radius - y);
                auto add_neighbours2 = _entity_tree.norm1_range_query(
                    fictive_position2,
                    std::max(fictive_radius, fictive_radius2));
                ent_neighbours.insert(ent_neighbours.end(),
                                      add_neighbours2.begin(),
                                      add_neighbours2.end());
            }
            if (y + radius > _height) {
                Eigen::Vector2d fictive_position2(x + _width, y - _height);
                float fictive_radius2(radius - (_height - y));
                auto add_neighbours2 = _entity_tree.norm1_range_query(
                    fictive_position2,
                    std::max(fictive_radius, fictive_radius2));
                ent_neighbours.insert(ent_neighbours.end(),
                                      add_neighbours2.begin(),
                                      add_neighbours2.end());
            }
        }
        if (x + radius > _width) {
            Eigen::Vector2d fictive_position(x - _width, y);
            float fictive_radius(radius - (_width - x));
            auto add_neighbours =
                _entity_tree.norm1_range_query(fictive_position, fictive_radius);
            ent_neighbours.insert(ent_neighbours.end(), add_neighbours.begin(),
                                  add_neighbours.end());
            if (y - radius < 0) {
                Eigen::Vector2d fictive_position2(x - _width, y + _height);
                float fictive_radius2(radius - y);
                auto add_neighbours2 = _entity_tree.norm1_range_query(
                    fictive_position2,
                    std::max(fictive_radius, fictive_radius2));
                ent_neighbours.insert(ent_neighbours.end(),
                                      add_neighbours2.begin(),
                                      add_neighbours2.end());
            }
            if (y + radius > _height) {
                Eigen::Vector2d fictive_position2(x - _width, y - _height);
                float fictive_radius2(radius - (_height - y));
                auto add_neighbours2 = _entity_tree.norm1_range_query(
                    fictive_position2,
                    std::max(fictive_radius, fictive_radius2));
                ent_neighbours.insert(ent_neighbours.end(),
                                      add_neighbours2.begin(),
                                      add_neighbours2.end());
            }
        }
        if (y - radius < 0) {
            Eigen::Vector2d fictive_position(x, y + _height);
            float fictive_radius(radius - y);
            auto add_neighbours =
                _entity_tree.norm1_range_query(fictive_position, fictive_radius);
            ent_neighbours.insert(ent_neighbours.end(), add_neighbours.begin(),
                                  add_neighbours.end());
        }
        if (y + radius > _height) {
            Eigen::Vector2d fictive_position(x, y - _height);
            float fictive_radius(radius - (_height - y));
            auto add_neighbours =
                _entity_tree.norm1_range_query(fictive_position, fictive_radius);
            ent_neighbours.insert(ent_neighbours.end(), add_neighbours.begin(),
                                  add_neighbours.end());
        }
        entity->neighbours().insert(entity->neighbours().end(),
                                           ent_neighbours.begin(),
                                           ent_neighbours.end());
        // Keep only unique references
        std::sort(entity->neighbours().begin(),
                  entity->neighbours().end());
        entity->neighbours().erase(
            std::unique(entity->neighbours().begin(),
                        entity->neighbours().end()),
            entity->neighbours().end());
    }
}

void World::call_entity_decision() {
    for (auto &&entity : _entity_list) {
        // Update the timestep of the Entity, necessary for computation
        entity->decision();
    }
}

void World::update_entity_and_renderer() {
    for (auto &&entity : _entity_list) {
        entity->update();
        Eigen::Vector2d screen_pos = convert(entity->pos());
        Eigen::Vector2d screen_size = convert(entity->size());
        if (_render_window != nullptr) {
            _render_window->add_FillRect_to_renderer(
                screen_pos(0), screen_pos(1), screen_size(0), screen_size(0),
                entity->color());
        }
    }
}

std::weak_ptr<Entity> World::add_entity(Entity::Type type, float x, float y) {
    int next_id;
    std::weak_ptr<Entity> result;
    try {
        next_id = _entity_count.at(type);
    } catch (const std::out_of_range &e) {
        _entity_count[type] = 0;
        next_id = 0;
    }

    if (x < 0 || y < 0) {
        auto p_newEnt = Entity::makeEntity(type, next_id, *this);
        _entity_list.push_back(std::move(p_newEnt));
        result = _entity_list.back();
    } else {
        auto p_newEnt = Entity::makeEntity(type, next_id, *this, x, y);
        _entity_list.push_back(std::move(p_newEnt));
        result = _entity_list.back();
    }
    _entity_count[type]++;
    return result;
}

std::weak_ptr<Entity> World::add_entity(Entity::Type type, float x, float y,
                                        float vx, float vy) {
    int next_id;
    std::weak_ptr<Entity> result;
    try {
        next_id = _entity_count.at(type);
    } catch (const std::out_of_range &e) {
        _entity_count[type] = 0;
        next_id = 0;
    }

    auto p_newEnt = Entity::makeEntity(type, next_id, *this, x, y, vx, vy);
    _entity_list.push_back(std::move(p_newEnt));
    result = _entity_list.back();
    _entity_count[type]++;
    return result;
}

std::weak_ptr<Entity> World::add_entity(std::string json_name, float x, float y,
                                        float vx, float vy) {
    std::weak_ptr<Entity> result;
    std::string full_path = std::string(DATA_DIR) + "entity/" + json_name;
    std::fstream fs;
    fs.open(full_path, std::ios::in);
    if (!fs.is_open()) {
        return result;
    } else {
        Json::Value json_root;
        fs >> json_root;

        Entity::Type entity_type;
        std::string entity_type_string = json_root.get("type", "").asString();
        if (entity_type_string == "Ant") {
            entity_type = Entity::Type::ANT;
        } else if (entity_type_string == "Food") {
            entity_type = Entity::Type::FOOD;
        } else {
            entity_type = Entity::Type::NONE;
        }

        int next_id;
        try {
            next_id = _entity_count.at(entity_type);
        } catch (const std::out_of_range &e) {
            _entity_count[entity_type] = 0;
            next_id = 0;
        }

        if (x < 0 || y < 0) {
            auto p_newEnt =
                Entity::makeEntity(entity_type, next_id, *this, json_root);
            _entity_list.push_back(std::move(p_newEnt));
            result = _entity_list.back();
        } else {
            auto p_newEnt = Entity::makeEntity(entity_type, next_id, *this,
                                               json_root, x, y, vx, vy);
            _entity_list.push_back(std::move(p_newEnt));
            result = _entity_list.back();
        }
        _entity_count[entity_type]++;
        return result;
    }
}

void World::serve_json_event(std::weak_ptr<WorldEvent> event) {
    auto p_event = event.lock();
    if (!p_event) {
        std::cerr << "World::serve_json_event : Event cannot be served as it "
                     "is not accessible anymore\n";
        return;
    }

    if (p_event->is_creation()) {
        auto p_creation = dynamic_cast<CreationEvent *>(p_event.get());
        if (p_creation->has_position()) {
            if (p_creation->has_velocity()) {
                add_entity(p_creation->json_template_name(),
                           p_creation->pos()(0), p_creation->pos()(1),
                           p_creation->vel()(0), p_creation->vel()(1));
            } else {
                add_entity(p_creation->json_template_name(),
                           p_creation->pos()(0), p_creation->pos()(1));
            }
        } else {
            add_entity(p_creation->json_template_name());
        }
        return;
    } else if (p_event->is_destruction()) {
        std::cerr
            << "World::serve_json_event : Destruction is not served yet\n";
        return;
    } else {
        std::cerr
            << "World::serve_json_event : This event type is not recognized\n";
        return;
    }
}
