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
#include "entity/ant/ant.h"
#include "entity/food/food.h"
#include "world/world.h"

std::string Entity::type_string() const {
    switch (_type) {
        case (Type::ANT):
            return "Ant";
            break;
        case (Type::FOOD):
            return "Food";
            break;
        case (Type::NONE):
        default:
            std::cerr << "The type has not been defined !!\n";
            return "";
    }
}

Entity::Entity(int i, World &world) : ent_id(i), parent_world(&world) {
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<double> width_dist(0, world._width);
    std::uniform_real_distribution<double> height_dist(0, world._height);

    _position(0) = width_dist(e1);
    _position(1) = height_dist(e1);
}

Entity::Entity(int i, World &world, Json::Value &&root) : Entity(i, world) {
    _json_root = std::move(root);
    read_from_json();
}

Entity::Entity(int i, World &world, float x, float y, float vx, float vy,
               float ax, float ay)
    : ent_id(i),
      parent_world(&world),
      _position(x, y),
      _velocity(vx, vy),
      _acceleration(ax, ay) {}

Entity::Entity(int i, World &world, Json::Value &&root, float x, float y,
               float vx, float vy, float ax, float ay)
    : Entity(i, world, x, y, vx, vy, ax, ay) {
    _json_root = std::move(root);
    read_from_json();
}

Entity::Entity(float x, float y, float vx, float vy, float ax, float ay)
    : _position(x, y), _velocity(vx, vy), _acceleration(ax, ay) {}

Eigen::Vector2d Entity::compute_friction_acceleration() {
    Eigen::Vector2d result = -1 * _velocity;
    result.normalize();
    float vel_norm = _velocity.norm();
    float scaling_factor = _friction_factor * vel_norm;

    if (scaling_factor >= _mass * vel_norm / parent_world->_time_step) {
        scaling_factor = vel_norm * _mass / parent_world->_time_step;
    }

    result *= scaling_factor / _mass;
    return result;
}

void Entity::decision() { _acceleration << 0, 0; }

void Entity::update() {
    _acceleration += compute_friction_acceleration();
    _velocity += parent_world->_time_step * _acceleration;
    _position += parent_world->_time_step * _velocity;
    parent_world->wrap_around(_position);

    _acceleration << 0, 0;
}

void Entity::print() const {
    std::cout << "***********************************\n";
    std::cout << "Position : " << _position(0) << "\t" << _position(1) << "\n";
    std::cout << "Velocity : " << _velocity(0) << "\t" << _velocity(1) << "\n";
    std::cout << "Acceleration : " << _acceleration(0) << "\t" << _acceleration(1)
              << "\n";
}

Entity::~Entity() {
    for (auto &&neighbour : _neighbours) {
        neighbour.reset();
    }
}

Eigen::Vector2d Entity::accel_towards(const Eigen::Vector2d &target_velocity) {
    float dt = parent_world->time_step();
    Eigen::Vector2d result = target_velocity - _velocity;
    result /= dt;
    return result;
}

bool Entity::operator<(const Entity &rhs) const {
    if (_type < rhs._type) {
        return true;
    }
    return ent_id < rhs.ent_id;
}

bool operator<(const std::weak_ptr<Entity> &lhs,
               const std::weak_ptr<Entity> &rhs) {
    return lhs.lock().get() < rhs.lock().get();
}

bool operator==(const std::weak_ptr<Entity> &lhs,
                const std::weak_ptr<Entity> &rhs) {
    return lhs.lock().get() == rhs.lock().get();
}

void Entity::update_json() const {
    _json_root["type"] = type_string();
    _json_root["id"] = ent_id;
    _json_root["vision"]["distance"] = _vision_distance;
    _json_root["size"][0] = _size(0);
    _json_root["size"][1] = _size(1);
    _json_root["world_situation"]["position"][0] = _position(0);
    _json_root["world_situation"]["position"][1] = _position(1);
    _json_root["world_situation"]["velocity"][0] = _velocity(0);
    _json_root["world_situation"]["velocity"][1] = _velocity(1);
    _json_root["world_situation"]["acceleration"][0] = _acceleration(0);
    _json_root["world_situation"]["acceleration"][1] = _acceleration(1);
    _json_root["mass"] = _mass;
    _json_root["max_acceleration"] = _max_acceleration;
    _json_root["friction_factor"] = _friction_factor;
}

void Entity::read_from_json() {
    if (_json_root["type"] == "Food") {
        _type = Type::FOOD;
    } else if (_json_root["type"] == "Ant") {
        _type = Type::ANT;
    } else {
        _type = Type::NONE;
    }
    // ent_id = json_root["id"].asInt();
    _vision_distance = _json_root["vision"]["distance"].asFloat();
    _size(0) = _json_root["size"][0].asDouble();
    _size(1) = _json_root["size"][1].asDouble();
    // position(0) = json_root["world_situation"]["position"][0].asDouble();
    // position(1) = json_root["world_situation"]["position"][1].asDouble();
    // velocity(0) = json_root["world_situation"]["velocity"][0].asDouble();
    // velocity(1) = json_root["world_situation"]["velocity"][1].asDouble();
    // acceleration(0) =
    // json_root["world_situation"]["acceleration"][0].asDouble();
    // acceleration(1) =
    // json_root["world_situation"]["acceleration"][1].asDouble();
    _mass = _json_root["mass"].asFloat();
    _max_acceleration = _json_root["max_acceleration"].asFloat();
    _friction_factor = _json_root["friction_factor"].asFloat();
}
