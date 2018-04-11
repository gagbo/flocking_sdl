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

#ifndef ENTITY_FOOD_FOOD_H_
#define ENTITY_FOOD_FOOD_H_
#include "../entity.h"
#include "jsoncpp/json/json.h"

class Food : public Entity {
 public:
    // Default constructor
    Food();
    // Default constructor that sets the world to live in
    Food(int i, World &parent_world);
    // Constructor that allows placement of the entity
    Food(int i, World &world, float x, float y, float vx = 0, float vy = 0);
    // Default constructor that sets the world to live in
    Food(int i, World &parent_world, Json::Value &root);
    // Constructor that allows placement of the entity
    Food(int i, World &world, Json::Value &root, float x, float y,
         float vx = 0, float vy = 0);
    // Constructor that allows World-less food
    Food(float x, float y, float vx = 0, float vy = 0);

    ~Food();

    inline float get_stock() const { return stock; }

 protected:
    float stock;
};

#endif  // ENTITY_FOOD_FOOD_H_
