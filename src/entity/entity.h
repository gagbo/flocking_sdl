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

#ifndef ENTITY_ENTITY_H_
#define ENTITY_ENTITY_H_
#include <Eigen/Dense>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <string>

class World;
class Ant;
class Food;

// Entity that lives in World and can move/should be displayed
class Entity {
 public:
    enum Type : int { NONE, ANT, FOOD };

    // Default constructor
    Entity();
    // Default constructor that points to the window to use to display
    Entity(int i, World &world);
    // Constructor that allows placement of the entity
    Entity(int i, World &world, float x, float y, float vx = 0, float vy = 0,
           float ax = 0, float ay = 0);

    // Has to be defined in header to allow auto resolution for World
    template <typename... Ts>
    static auto makeEntity(Type type, Ts &&... params) {
        auto delEntity = [](Entity *pEntity) { delete pEntity; };

        std::unique_ptr<Entity, decltype(delEntity)> pEnt(nullptr, delEntity);

        switch (type) {
            case (Entity::Type::ANT):
                pEnt.reset(new Ant(std::forward<Ts>(params)...));
                break;
            case (Entity::Type::FOOD):
                pEnt.reset(new Food(std::forward<Ts>(params)...));
                break;
            default:
                break;
        }

        return pEnt;
    }

    virtual ~Entity();

    // decide where to go by setting acceleration accordingly
    virtual void decision();

    // update the position according to World::time_step and add shape to
    // renderer for next frame
    void update();

    // Print debug info about the entity
    void print() const;

    // Accessors
    inline const Eigen::Vector2d &get_pos() const { return position; }
    inline const Eigen::Vector2d &get_vel() const { return velocity; }
    inline const Eigen::Vector2d &get_size() const { return size; }
    inline int id() const { return ent_id; }
    inline int *get_color() { return color; }
    inline Type get_type() { return type; }
    std::string get_type_string() const;

 protected:
    // Type of the entity
    Type type;
    // Id of the entity
    int ent_id;
    // Pointer to the parent World
    World *parent_world;
    // Size of the bounding rect that represents Entity
    Eigen::Vector2d size{5};
    // Position of the entity
    Eigen::Vector2d position;
    // Velocity of the entity
    Eigen::Vector2d velocity{0, 0};
    // Acceleration of the entity
    Eigen::Vector2d acceleration{0, 0};
    // Mass of the entity
    float mass{1.0};
    // Maximum acceleration possible for Entity
    float max_acceleration{std::numeric_limits<float>::max()};
    // Coefficient of friction (drag)
    float friction_factor{0};
    // Color used to display entity (in hex RGBA)
    int color[4];

    // Compute a linear then quadratic friction acceleration
    Eigen::Vector2d get_friction_acceleration();
};

#endif  // ENTITY_ENTITY_H_
