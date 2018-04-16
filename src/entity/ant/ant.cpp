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
    _type = Entity::Type::ANT;
    _mass = 1;
    _friction_factor = 0;
    set_color(default_color);
    _vision_distance = 125;
}

Ant::Ant(int i, World &parent_world) : Entity(i, parent_world) {
    _type = Entity::Type::ANT;
    _mass = 1;
    _friction_factor = 0;
    set_color(default_color);
    _vision_distance = 125;
}

Ant::Ant(int i, World &parent_world, Json::Value &root)
    : Entity(i, parent_world, std::move(root)) {
    read_from_json();
}

Ant::Ant(int i, World &world, float x, float y, float vx, float vy, float ax,
         float ay)
    : Entity(i, world, x, y, vx, vy, ax, ay) {
    _type = Entity::Type::ANT;
    _mass = 1;
    _friction_factor = 0;
    set_color(default_color);
    _vision_distance = 125;
}

Ant::Ant(int i, World &world, Json::Value &root, float x, float y, float vx,
         float vy, float ax, float ay)
    : Entity(i, world, std::move(root), x, y, vx, vy, ax, ay) {
    read_from_json();
}

Ant::Ant(float x, float y, float vx, float vy, float ax, float ay)
    : Entity(x, y, vx, vy, ax, ay) {
    _type = Entity::Type::ANT;
    _mass = 1;
    _friction_factor = 0;
    set_color(default_color);
    _vision_distance = 125;
}

void Ant::decision() {
    filter_neighbours();
    if (_neighbours.size() <= 1) {
        set_color(blind_color);
        _acceleration << 0, 0;
    } else {
        set_color(default_color);
        Eigen::Vector2d decided_velocity(0.0, 0.0);
        decided_velocity = _cohesion_weight * decision_cohesion_velocity();
        decided_velocity += _alignment_weight * decision_alignment_velocity();
        decided_velocity += _separation_weight * decision_separation_velocity();

        _acceleration = accel_towards(decided_velocity);
        cap_acceleration();
    }
}

void Ant::filter_neighbours() {
    if (_velocity.norm() == 0) {
        filter_neighbours_standing();
    } else {
        filter_neighbours_moving();
    }
}

void Ant::filter_neighbours_standing() {
    _neighbours.erase(
        std::remove_if(_neighbours.begin(), _neighbours.end(),
                       [&](std::weak_ptr<Entity> &neigh) -> bool {
                           if (auto spt = neigh.lock()) {
                               Eigen::Vector2d vec = spt->pos() - _position;
                               return vec.squaredNorm() >
                                      _vision_distance * _vision_distance * 4;
                           } else {
                               return true;
                           }
                       }),
        _neighbours.end());
}

void Ant::filter_neighbours_moving() {
    _neighbours.erase(std::remove_if(_neighbours.begin(), _neighbours.end(),
                                    [&](std::weak_ptr<Entity> &neigh) -> bool {
                                        if (auto spt = neigh.lock()) {
                                            Eigen::Vector2d vec =
                                                spt->pos() - _position;
                                            return !is_in_vision_triangle(vec);
                                        } else {
                                            return true;
                                        }
                                    }),
                     _neighbours.end());
}

bool Ant::is_in_vision_triangle(const Eigen::Vector2d &vec) const {
    if (vec.squaredNorm() > _vision_distance * _vision_distance) {
        return false;
    }
    if (vec.squaredNorm() == 0) {
        return true;
    }
    float cos_theta = (vec.dot(_velocity)) / (vec.norm() * _velocity.norm());

    return std::acos(cos_theta) < (_vision_angle_degrees * M_PI / 180.0);
}

Eigen::Vector2d Ant::decision_separation_velocity() const {
    Eigen::Vector2d desired(0, 0);

    for (auto &&item : _neighbours) {
        if (auto spt = item.lock()) {
            if (spt.get() == this) {
                continue;
            }
            Eigen::Vector2d to_rival =
                parent_world->point_to(_position, spt->pos());
            Eigen::Vector2d weighted_diff = -1 * to_rival;
            float dist = weighted_diff.norm();
            weighted_diff.normalize();
            weighted_diff /=
                pow((dist + _vision_distance / 4.0), _separation_potential_exp);
            desired += weighted_diff;
        }
    }
    desired.normalize();
    desired *= _cruise_speed / parent_world->time_step();

    return desired;
}

Eigen::Vector2d Ant::decision_alignment_velocity() const {
    Eigen::Vector2d desired(0, 0);

    int considered_neigbours = 0;
    for (auto &&item : _neighbours) {
        if (auto spt = item.lock()) {
            ++considered_neigbours;
            desired += spt->vel();
        }
    }

    if (considered_neigbours > 0) {
        desired /= considered_neigbours;
    }

    desired.normalize();
    desired *= _cruise_speed / parent_world->time_step();

    return desired;
}

Eigen::Vector2d Ant::decision_cohesion_velocity() const {
    Eigen::Vector2d desired(0, 0);

    for (auto &&item : _neighbours) {
        if (auto spt = item.lock()) {
            desired += parent_world->point_to(_position, spt->pos());
        }
    }

    desired.normalize();
    desired *= _cruise_speed / parent_world->time_step();

    return desired;
}

void Ant::cap_acceleration() {
    float norm = _acceleration.norm();
    if (norm > _max_acceleration) {
        set_color(capped_force_color);
        _acceleration.normalize();
        _acceleration = _max_acceleration * _acceleration;
    }
}

Ant::~Ant() {}

void Ant::update_json() const {
    Entity::update_json();
    _json_root["colors"]["default"][0] = default_color[0];
    _json_root["colors"]["default"][1] = default_color[1];
    _json_root["colors"]["default"][2] = default_color[2];
    _json_root["colors"]["default"][3] = default_color[3];
    _json_root["colors"]["blind"][0] = blind_color[0];
    _json_root["colors"]["blind"][1] = blind_color[1];
    _json_root["colors"]["blind"][2] = blind_color[2];
    _json_root["colors"]["blind"][3] = blind_color[3];
    _json_root["colors"]["capped_force"][0] = capped_force_color[0];
    _json_root["colors"]["capped_force"][1] = capped_force_color[1];
    _json_root["colors"]["capped_force"][2] = capped_force_color[2];
    _json_root["colors"]["capped_force"][3] = capped_force_color[3];
    _json_root["decision_weights"]["cohesion"] = _cohesion_weight;
    _json_root["decision_weights"]["alignment"] = _alignment_weight;
    _json_root["decision_weights"]["separation"] = _separation_weight;
    _json_root["separation_potential_exponent"] = _separation_potential_exp;
    _json_root["cruise_speed"] = _cruise_speed;
    _json_root["vision"]["angle_degrees"] = _vision_angle_degrees;
    return;
}

void Ant::read_from_json() {
    Entity::read_from_json();
    default_color[0] = _json_root["colors"]["default"][0].asInt();
    default_color[1] = _json_root["colors"]["default"][1].asInt();
    default_color[2] = _json_root["colors"]["default"][2].asInt();
    default_color[3] = _json_root["colors"]["default"][3].asInt();
    blind_color[0] = _json_root["colors"]["blind"][0].asInt();
    blind_color[1] = _json_root["colors"]["blind"][1].asInt();
    blind_color[2] = _json_root["colors"]["blind"][2].asInt();
    blind_color[3] = _json_root["colors"]["blind"][3].asInt();
    capped_force_color[0] = _json_root["colors"]["capped_force"][0].asInt();
    capped_force_color[1] = _json_root["colors"]["capped_force"][1].asInt();
    capped_force_color[2] = _json_root["colors"]["capped_force"][2].asInt();
    capped_force_color[3] = _json_root["colors"]["capped_force"][3].asInt();
    _cohesion_weight = _json_root["decision_weights"]["cohesion"].asFloat();
    _alignment_weight = _json_root["decision_weights"]["alignment"].asFloat();
    _separation_weight = _json_root["decision_weights"]["separation"].asFloat();
    _separation_potential_exp =
        _json_root["separation_potential_exponent"].asFloat();
    _cruise_speed = _json_root["cruise_speed"].asFloat();
    _vision_angle_degrees = _json_root["vision"]["angle_degrees"].asFloat();
    return;
}
