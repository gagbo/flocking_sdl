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

#ifndef UI_INPUT_JSONEVENT_H
#define UI_INPUT_JSONEVENT_H
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "jsoncpp/json/json.h"

class WorldEvent {
 public:
    WorldEvent() = default;
    virtual ~WorldEvent(){};

    //! Return true if WorldEvent is CreationEvent
    inline bool is_creation() const { return _is_creation; }
    //! Return true if WorldEvent is DestructionEvent
    inline bool is_destruction() const { return _is_destruction; }
    //! Return the associated time_stamp
    inline float time_stamp() const { return _time_stamp; }

 protected:
    bool _is_creation{false};
    bool _is_destruction{false};
    float _time_stamp{-1};
};

bool operator<(const std::weak_ptr<WorldEvent>& lhs,
               const std::weak_ptr<WorldEvent>& rhs);

bool operator<(const std::shared_ptr<WorldEvent>& lhs,
               const std::shared_ptr<WorldEvent>& rhs);

class WorldEventsList {
 public:
    //! Default constructor
    WorldEventsList() = default;
    //! Constructor from a file name
    WorldEventsList(std::string file_name);
    //! Constructor from a input stream.
    //  The input stream will be considered JSON
    WorldEventsList(std::istream& in);

    // Interact with the list members
    //! Erase and update _list
    void construct_from_json(const Json::Value& root);
    //! Appends all the events to _ list without duplicate checking
    void append_from_json(const Json::Value& root);
    //! istream 'constructor' helper
    void read_istream(std::istream& in, bool append = false);

    // Accessor
    auto inline list() const { return _list; }

    //! Get the list of events between a starting time_stamp (inclusive)
    //  and an ending time_stamp (exclusive)
    std::vector<std::weak_ptr<WorldEvent>> events_in_time_frame(
        float interval_start, float interval_end);

    ~WorldEventsList();

 protected:
    std::vector<std::shared_ptr<WorldEvent>> _list{};

    //! Mark if list is already time sorted
    bool _sorted_list{false};

    void sort_events_list();
};

class CreationEvent : public WorldEvent {
 public:
    CreationEvent() = default;
    CreationEvent(float& time_stamp, const Json::Value& root);
    ~CreationEvent();

    inline std::string json_template_name() const { return _template_name; }
    inline const Eigen::Vector2d& pos() const { return _pos; }
    inline const Eigen::Vector2d& vel() const { return _vel; }
    inline const Eigen::Vector2d& acc() const { return _acc; }

    inline bool has_position() const { return _has_position; }
    inline bool has_velocity() const { return _has_velocity; }
    inline bool has_acceleration() const { return _has_acceleration; }

 protected:
    //! "type" json_field -> links to a filename for reading the entity type
    std::string _template_name{""};
    //! Mark if the CreationEvent wants to set position (random if not set)
    bool _has_position{false};
    Eigen::Vector2d _pos{-1, -1};
    //! Mark if the CreationEvent wants to set velocity (random if not set)
    bool _has_velocity{false};
    Eigen::Vector2d _vel{-1, -1};
    //! Mark if the CreationEvent wants to set acceleration (random if not set)
    bool _has_acceleration{false};
    Eigen::Vector2d _acc{-1, -1};
};

class DestructionEvent : public WorldEvent {
 public:
    DestructionEvent() = default;
    DestructionEvent(float& time_stamp, const Json::Value& root);
    ~DestructionEvent();

 protected:
    //! "type" json_field -> links to a typeString()
    std::string _entity_type{""};
    //! id of the entity to kill
    int _id{0};
};

#endif  // UI_INPUT_JSONEVENT_H
