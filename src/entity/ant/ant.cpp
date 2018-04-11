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

#include <algorithm>
#include "ant.h"
#include "world/world.h"

Ant::Ant() : Entity() {
    type = Entity::Type::ANT;
    mass = 1;
    friction_factor = 0;
    set_color(default_color);
    vision_distance = 125;
}

Ant::Ant(int i, World &parent_world) : Entity(i, parent_world) {
    type = Entity::Type::ANT;
    mass = 1;
    friction_factor = 0;
    set_color(default_color);
    vision_distance = 125;
}

Ant::Ant(int i, World &parent_world, Json::Value &root)
    : Entity(i, parent_world, std::move(root)) {
    read_from_json();
}

Ant::Ant(int i, World &world, float x, float y, float vx, float vy, float ax,
         float ay)
    : Entity(i, world, x, y, vx, vy, ax, ay) {
    type = Entity::Type::ANT;
    mass = 1;
    friction_factor = 0;
    set_color(default_color);
    vision_distance = 125;
}

Ant::Ant(int i, World &world, Json::Value &root, float x, float y, float vx,
         float vy, float ax, float ay)
    : Entity(i, world, std::move(root), x, y, vx, vy, ax, ay) {
    read_from_json();
}

Ant::Ant(float x, float y, float vx, float vy, float ax, float ay)
    : Entity(x, y, vx, vy, ax, ay) {
    type = Entity::Type::ANT;
    mass = 1;
    friction_factor = 0;
    set_color(default_color);
    vision_distance = 125;
}

void Ant::decision() {
    filter_neighbours();
    if (neighbours.size() <= 1) {
        set_color(blind_color);
        acceleration << 0, 0;
    } else {
        set_color(default_color);
        Eigen::Vector2d decided_velocity(0.0, 0.0);
        decided_velocity = cohesion_weight * decision_cohesion_velocity();
        decided_velocity += alignment_weight * decision_alignment_velocity();
        decided_velocity += separation_weight * decision_separation_velocity();

        acceleration = accel_towards(decided_velocity);
        cap_acceleration();
    }
}

void Ant::filter_neighbours() {
    if (velocity.norm() == 0) {
        filter_neighbours_standing();
    } else {
        filter_neighbours_moving();
    }
}

void Ant::filter_neighbours_standing() {
    neighbours.erase(
        std::remove_if(neighbours.begin(), neighbours.end(),
                       [&](std::weak_ptr<Entity> &neigh) -> bool {
                           if (auto spt = neigh.lock()) {
                               Eigen::Vector2d vec = spt->get_pos() - position;
                               return vec.squaredNorm() >
                                      vision_distance * vision_distance * 4;
                           } else {
                               return true;
                           }
                       }),
        neighbours.end());
}

void Ant::filter_neighbours_moving() {
    neighbours.erase(std::remove_if(neighbours.begin(), neighbours.end(),
                                    [&](std::weak_ptr<Entity> &neigh) -> bool {
                                        if (auto spt = neigh.lock()) {
                                            Eigen::Vector2d vec =
                                                spt->get_pos() - position;
                                            return !is_in_vision_triangle(vec);
                                        } else {
                                            return true;
                                        }
                                    }),
                     neighbours.end());
}

bool Ant::is_in_vision_triangle(const Eigen::Vector2d &vec) const {
    if (vec.squaredNorm() > vision_distance * vision_distance) {
        return false;
    }
    if (vec.squaredNorm() == 0) {
        return true;
    }
    float cos_theta = (vec.dot(velocity)) / (vec.norm() * velocity.norm());

    return std::acos(cos_theta) < (vision_angle_degrees * M_PI / 180.0);
}

Eigen::Vector2d Ant::decision_separation_velocity() const {
    Eigen::Vector2d desired(0, 0);

    for (auto &&item : neighbours) {
        if (auto spt = item.lock()) {
            if (spt.get() == this) {
                continue;
            }
            Eigen::Vector2d to_rival =
                parent_world->point_to(position, spt->get_pos());
            Eigen::Vector2d weighted_diff = -1 * to_rival;
            float dist = weighted_diff.norm();
            weighted_diff.normalize();
            weighted_diff /=
                pow((dist + vision_distance / 4.0), separation_potential_exp);
            desired += weighted_diff;
        }
    }
    desired.normalize();
    desired *= cruise_speed / parent_world->get_time_step();

    return desired;
}

Eigen::Vector2d Ant::decision_alignment_velocity() const {
    Eigen::Vector2d desired(0, 0);

    int considered_neigbours = 0;
    for (auto &&item : neighbours) {
        if (auto spt = item.lock()) {
            ++considered_neigbours;
            desired += spt->get_vel();
        }
    }

    if (considered_neigbours > 0) {
        desired /= considered_neigbours;
    }

    desired.normalize();
    desired *= cruise_speed / parent_world->get_time_step();

    return desired;
}

Eigen::Vector2d Ant::decision_cohesion_velocity() const {
    Eigen::Vector2d desired(0, 0);

    for (auto &&item : neighbours) {
        if (auto spt = item.lock()) {
            desired += parent_world->point_to(position, spt->get_pos());
        }
    }

    desired.normalize();
    desired *= cruise_speed / parent_world->get_time_step();

    return desired;
}

void Ant::cap_acceleration() {
    float norm = acceleration.norm();
    if (norm > max_acceleration) {
        set_color(capped_force_color);
        acceleration.normalize();
        acceleration = max_acceleration * acceleration;
    }
}

Ant::~Ant() {}

void Ant::update_json() const {
    Entity::update_json();
    json_root["colors"]["default"][0] = default_color[0];
    json_root["colors"]["default"][1] = default_color[1];
    json_root["colors"]["default"][2] = default_color[2];
    json_root["colors"]["default"][3] = default_color[3];
    json_root["colors"]["blind"][0] = blind_color[0];
    json_root["colors"]["blind"][1] = blind_color[1];
    json_root["colors"]["blind"][2] = blind_color[2];
    json_root["colors"]["blind"][3] = blind_color[3];
    json_root["colors"]["capped_force"][0] = capped_force_color[0];
    json_root["colors"]["capped_force"][1] = capped_force_color[1];
    json_root["colors"]["capped_force"][2] = capped_force_color[2];
    json_root["colors"]["capped_force"][3] = capped_force_color[3];
    json_root["decision_weights"]["cohesion"] = cohesion_weight;
    json_root["decision_weights"]["alignment"] = alignment_weight;
    json_root["decision_weights"]["separation"] = separation_weight;
    json_root["separation_potential_exponent"] = separation_potential_exp;
    json_root["cruise_speed"] = cruise_speed;
    json_root["vision"]["angle_degrees"] = vision_angle_degrees;
    return;
}

void Ant::read_from_json() {
    Entity::read_from_json();
    default_color[0] = json_root["colors"]["default"][0].asInt();
    default_color[1] = json_root["colors"]["default"][1].asInt();
    default_color[2] = json_root["colors"]["default"][2].asInt();
    default_color[3] = json_root["colors"]["default"][3].asInt();
    blind_color[0] = json_root["colors"]["blind"][0].asInt();
    blind_color[1] = json_root["colors"]["blind"][1].asInt();
    blind_color[2] = json_root["colors"]["blind"][2].asInt();
    blind_color[3] = json_root["colors"]["blind"][3].asInt();
    capped_force_color[0] = json_root["colors"]["capped_force"][0].asInt();
    capped_force_color[1] = json_root["colors"]["capped_force"][1].asInt();
    capped_force_color[2] = json_root["colors"]["capped_force"][2].asInt();
    capped_force_color[3] = json_root["colors"]["capped_force"][3].asInt();
    cohesion_weight = json_root["decision_weights"]["cohesion"].asFloat();
    alignment_weight = json_root["decision_weights"]["alignment"].asFloat();
    separation_weight = json_root["decision_weights"]["separation"].asFloat();
    separation_potential_exp =
        json_root["separation_potential_exponent"].asFloat();
    cruise_speed = json_root["cruise_speed"].asFloat();
    vision_angle_degrees = json_root["vision"]["angle_degrees"].asFloat();
    return;
}
