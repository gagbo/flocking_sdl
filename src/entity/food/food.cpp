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

#include "food.h"

Food::Food() : Entity() {
    _type = Entity::Type::FOOD;
    _mass = 1;
}

Food::Food(int i, World &parent_world) : Entity(i, parent_world) {
    _type = Entity::Type::FOOD;
    _mass = 1;
}

Food::Food(int i, World &world, float x, float y, float vx, float vy)
    : Entity(i, world, x, y, vx, vy, 0, 0) {
    _type = Entity::Type::FOOD;
    _mass = 1;
}

Food::Food(int i, World &parent_world, Json::Value &root)
    : Entity(i, parent_world, std::move(root)) {
    _mass = 1;
}

Food::Food(int i, World &world, Json::Value &root, float x, float y, float vx,
           float vy)
    : Entity(i, world, std::move(root), x, y, vx, vy, 0, 0) {
    _mass = 1;
}

Food::Food(float x, float y, float vx, float vy) : Entity(x, y, vx, vy, 0, 0) {
    _type = Entity::Type::FOOD;
    _mass = 1;
}

Food::~Food() {}
