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

std::string Entity::get_type_string() const {
    switch (type) {
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
    std::uniform_real_distribution<double> width_dist(0, world.width);
    std::uniform_real_distribution<double> height_dist(0, world.height);

    position(0) = width_dist(e1);
    position(1) = height_dist(e1);
}

Entity::Entity(int i, World &world, float x, float y, float vx, float vy,
               float ax, float ay)
    : ent_id(i),
      parent_world(&world),
      position(x, y),
      velocity(vx, vy),
      acceleration(ax, ay) {}

Entity::Entity(float x, float y, float vx, float vy, float ax, float ay)
    : position(x, y), velocity(vx, vy), acceleration(ax, ay) {}

Eigen::Vector2d Entity::get_friction_acceleration() {
    Eigen::Vector2d result = -1 * velocity;
    result.normalize();
    float vel_norm = velocity.norm();
    float scaling_factor = friction_factor * vel_norm;

    if (scaling_factor >= mass * vel_norm / parent_world->time_step) {
        scaling_factor = vel_norm * mass / parent_world->time_step;
    }

    result *= scaling_factor / mass;
    return result;
}

void Entity::decision() { acceleration << 0, 0; }

void Entity::update() {
    acceleration += get_friction_acceleration();
    velocity += parent_world->time_step * acceleration;
    position += parent_world->time_step * velocity;
    parent_world->wrap_around(position);

    acceleration << 0, 0;
}

void Entity::print() const {
    std::cout << "***********************************\n";
    std::cout << "Position : " << position(0) << "\t" << position(1) << "\n";
    std::cout << "Velocity : " << velocity(0) << "\t" << velocity(1) << "\n";
    std::cout << "Acceleration : " << acceleration(0) << "\t" << acceleration(1)
              << "\n";
}

Entity::~Entity() {
    for (auto &&neighbour : neighbours) {
        neighbour.reset();
    }
}

Eigen::Vector2d Entity::accel_towards(const Eigen::Vector2d &target_velocity) {
    float dt = parent_world->get_time_step();
    Eigen::Vector2d result = target_velocity - velocity;
    result /= dt;
    return result;
}

bool Entity::operator<(const Entity &rhs) const {
    if (type < rhs.type) {
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
    json_root["type"] = get_type_string();
    json_root["id"] = ent_id;
    json_root["vision"]["distance"] = vision_distance;
    json_root["size"]["X"] = size(0);
    json_root["size"]["Y"] = size(1);
    json_root["world_situation"]["position"]["X"] = position(0);
    json_root["world_situation"]["position"]["Y"] = position(1);
    json_root["world_situation"]["velocity"]["X"] = velocity(0);
    json_root["world_situation"]["velocity"]["Y"] = velocity(1);
    json_root["world_situation"]["acceleration"]["X"] = acceleration(0);
    json_root["world_situation"]["acceleration"]["Y"] = acceleration(1);
    json_root["mass"] = mass;
    json_root["max_acceleration"] = max_acceleration;
    json_root["friction_factor"] = friction_factor;
}
