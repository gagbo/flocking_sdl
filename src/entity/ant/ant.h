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

    // Sets acceleration according to the decision of the ant
    void decision();

    static int default_color[4];

 protected:
    float vision_distance;
    float vision_angle_degrees;

    void cap_acceleration();
    void cap_force(float max_force);
};

#endif  // ENTITY_ANT_ANT_H_
