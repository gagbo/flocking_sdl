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
#include <string>
#include <vector>

#include "entity/entity.h"  // Necessary here to fully declare Entity::Type
#include "kdtree.h"
#include "ui/input/json_event.h"

#define DEFAULT_WORLD_WIDTH 640
#define DEFAULT_WORLD_HEIGHT 480
#define DEFAULT_PIX_WIDTH 640
#define DEFAULT_PIX_HEIGHT 480
#define DEFAULT_TIME_STEP 1.0

class MainWindow;
template class KDTree<Entity>;

// A public struct that contains world-related helpers/definitions
class World {
 public:
    World() = default;
    World(int w, int h, float dt);
    ~World();

    // Width of the world in arbitrary unit
    int _width{DEFAULT_WORLD_WIDTH};
    // Height of the world in arbitrary unit
    int _height{DEFAULT_WORLD_HEIGHT};
    // Width of the world in pixels
    int _width_in_px{DEFAULT_PIX_WIDTH};
    // Height of the world in pixels
    int _height_in_px{DEFAULT_PIX_HEIGHT};
    // Time step for the physics engine
    float _time_step{DEFAULT_TIME_STEP};

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
    std::weak_ptr<Entity> add_entity(Entity::Type type, float x = -1,
                                     float y = -1);
    // Overload to specify velocity
    std::weak_ptr<Entity> add_entity(Entity::Type type, float x, float y,
                                     float vx, float vy);
    // Add a new entity to the world by naming a json_template
    // Location is expected to lie in [0;width] X [0;height]
    // If it is not, location may be randomized back inside
    std::weak_ptr<Entity> add_entity(std::string json_name, float x = -1,
                                     float y = -1, float vx = 0, float vy = 0);

    // Add events from json input stream, with default behaviour of overwriting
    // current event list
    inline void add_events(std::istream &in, bool append = false) {
        _events.read_istream(in, append);
    }

    // Calls update on every entity and then send everything to renderer
    void update();
    // Find and serve all new json events that happened
    void find_and_serve_new_events();
    // Update the k-d tree
    void update_tree();
    // Compute the neighbourhoods of each entity
    void update_entity_neighbourhoods();
    // Call the decision method of each entity (alone in its loop because
    // Entity::update changes position while we're looping
    void call_entity_decision();
    // Update each entity
    void update_entity_and_renderer();

    // Serve a pointed-to event
    void serve_json_event(std::weak_ptr<WorldEvent> event);

    // Accessors
    MainWindow &get_mut_window();
    inline const auto &entity_list() const { return _entity_list; }
    inline float time_step() const { return _time_step; }
    inline double time() const { return _time; }

 protected:
    std::vector<std::shared_ptr<Entity>> _entity_list{};
    KDTree<Entity> _entity_tree{};
    WorldEventsList _events{};
    std::map<Entity::Type, int> _entity_count;
    // Pointer to the MainWindow on which to draw
    MainWindow *_render_window{nullptr};
    // Elapsed time
    double _time{0};
};

#endif  // WORLD_WORLD_H_
