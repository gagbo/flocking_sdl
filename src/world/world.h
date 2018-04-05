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

#ifndef WORLD_WORLD_H_
#define WORLD_WORLD_H_
#include <Eigen/Dense>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "entity/entity.h"  // Necessary here to fully declare Entity::Type
#include "kdtree.h"

class MainWindow;
template class KDTree<Entity>;

// A public struct that contains world-related helpers/definitions
class World {
 public:
    World();
    World(int w, int h, float dt);
    ~World();

    // Width of the world in arbitrary unit
    int width;
    // Height of the world in arbitrary unit
    int height;
    // Width of the world in pixels
    int width_in_px;
    // Height of the world in pixels
    int height_in_px;
    // Time step for the physics engine
    float time_step;

    // Setter for the MainWindow on which to draw
    void set_render_window(MainWindow &window);
    // Setter for the world size
    void set_world_size(int w, int h);
    // Setter for the time step
    void set_time_step(float t);
    // Translate position in place so the world wraps around edges
    void wrap_around(Eigen::Vector2d &position);
    // Translate arbitrary unit to pixels position for the renderer
    Eigen::Vector2d convert(const Eigen::Vector2d &position) const;
    // Computes the vector to go from tail to head
    // This only work on 'wrapped_around' vectors, for which coord
    //   lie in [0 ; width] x [0 ; height]
    Eigen::Vector2d point_to(const Eigen::Vector2d &tail,
                             const Eigen::Vector2d &head);

    // Add a new entity to the world, with an optional location
    // Location is expected to lie in [0;width] X [0;height]
    // If it is not, location may be randomized back inside
    void add_entity(Entity::Type type, float x = -1, float y = -1);

    // Calls update on every entity and then send everything to renderer
    void update();

    // Accessors
    MainWindow &get_mut_window();
    inline const auto &get_entity_list() const { return entity_list; }

 protected:
    std::vector<std::shared_ptr<Entity>> entity_list;
    KDTree<Entity> entity_tree;
    std::map<Entity::Type, int> entity_count;
    // Pointer to the MainWindow on which to draw
    MainWindow *render_window;

    // Update the k-d tree
    void update_tree();
    // Compute the neighbourhoods of each entity
    void update_entity_neighbourhoods();
    // Call the decision method of each entity (alone in its loop because
    // Entity::update changes position while we're looping
    void call_entity_decision();
    // Update each entity
    void update_entity_and_renderer();
};

#endif  // WORLD_WORLD_H_
