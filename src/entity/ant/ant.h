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

#ifndef ENTITY_ANT_ANT_H_
#define ENTITY_ANT_ANT_H_
#include "../entity.h"

class Ant : public Entity {
 public:
    // Default constructor
    Ant();
    // Default constructor that sets the world to live in
    Ant(int i, World &parent_world);
    // Constructor that allows placement of the entity
    Ant(int i, World &world, float x, float y, float vx = 0, float vy = 0,
        float ax = 0, float ay = 0);
    // Constructor that allows World-less Ant
    Ant(float x, float y, float vx = 0, float vy = 0, float ax = 0,
        float ay = 0);

    ~Ant();

    inline void set_vision_angle_deg(float d) { vision_angle_degrees = d; }
    inline void set_cruise_speed(float cs) { cruise_speed = cs; }
    inline void set_separation_exp(float exp) {
        separation_potential_exp = exp;
    }
    inline void set_cohesion_weight(float wt) { cohesion_weight = wt; }
    inline void set_alignment_weight(float wt) { alignment_weight = wt; }
    inline void set_separation_weight(float wt) { separation_weight = wt; }
    inline void set_max_force(float max_force) {
        max_acceleration = max_force / mass;
    }

    inline float get_cruise_speed() const { return cruise_speed; }
    inline float get_max_force() const { return mass * max_acceleration; }

    // Sets acceleration according to the decision of the ant
    void decision();
    Eigen::Vector2d decision_cohesion_velocity() const;
    Eigen::Vector2d decision_alignment_velocity() const;
    Eigen::Vector2d decision_separation_velocity() const;
    // Filter the neighbour list so only visible ones remain
    void filter_neighbours();
    // Return true if vec is in the triangle that is vision_angle_degrees on
    // each side of velocity, with length <= vision_distance;
    bool is_in_vision_triangle(const Eigen::Vector2d &vec) const;

    int default_color[4]{0x22, 0xA0, 0x22, 0xFF};
    int blind_color[4]{0xA0, 0x22, 0x22, 0xFF};
    int capped_force_color[4]{0xA0, 0x22, 0xA0, 0xFF};

 protected:
    float vision_angle_degrees{60};
    float cruise_speed{10};
    float separation_potential_exp{0.5};

    float cohesion_weight{0.1};
    float alignment_weight{0.6};
    float separation_weight{0.3};

    void cap_acceleration();
    void cap_force(float max_force);

    void filter_neighbours_standing();
    void filter_neighbours_moving();
};

#endif  // ENTITY_ANT_ANT_H_
