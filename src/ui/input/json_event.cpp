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
#include "FlockingConfig.h"
#include "json_event.h"

WorldEventsList::WorldEventsList(std::string file_name) {

    if (0 != file_name.compare(file_name.length() - 5, 5, ".json")) {
        return;
    }

    std::fstream json_stream;
    json_stream.open(std::string(DATA_DIR) + "events/" + file_name,
                     std::ios::in);
    if (!json_stream.is_open()) {
        std::cerr << "Error while opening " << file_name << "\n";
        return;
    }

    read_istream(json_stream);
}

WorldEventsList::WorldEventsList(std::istream& in) { read_istream(in); }

void WorldEventsList::read_istream(std::istream& in, bool append) {
    Json::Value root;
    in >> root;
    if (append) {
        append_from_json(root);
    } else {
        construct_from_json(root);
    }
}

void WorldEventsList::construct_from_json(const Json::Value& root) {
    _list.clear();
    append_from_json(root);
}

void WorldEventsList::append_from_json(const Json::Value& root) {
    // We don't check for "events" key since it's the responsibility of
    // a (hypothetic) schema validator
    const Json::Value events = root["events"];
    for (unsigned int i = 0; i < events.size(); ++i) {
        float time_stamp = events[i]["time_stamp"].asFloat();

        if (events[i]["creation"]) {
            _list.emplace_back(std::make_shared<CreationEvent>(
                time_stamp, events[i]["creation"]));
            _sorted_list = false;
        }
        if (events[i]["destruction"]) {
            _list.emplace_back(std::make_shared<DestructionEvent>(
                time_stamp, events[i]["destruction"]));
            _sorted_list = false;
        }
    }
}

void WorldEventsList::sort_events_list() {
    if (_sorted_list) {
        return;
    }

    std::sort(_list.begin(), _list.end());
    _sorted_list = true;
}

WorldEventsList::~WorldEventsList() { _list.clear(); }

std::vector<std::weak_ptr<WorldEvent>> WorldEventsList::events_in_time_frame(
    float interval_start, float interval_end) {
    std::vector<std::weak_ptr<WorldEvent>> result;
    if (!_sorted_list) {
        sort_events_list();
    }

    auto first_event = std::find_if(_list.begin(), _list.end(), [&](auto item) {
        return item->time_stamp() >= interval_start;
    });
    auto last_event = std::find_if(first_event, _list.end(), [&](auto item) {
        return item->time_stamp() >= interval_end;
    });

    result.insert(result.begin(), first_event, last_event);

    return result;
}

CreationEvent::CreationEvent(float& time_stamp, const Json::Value& root)
    : CreationEvent() {
    _is_creation = true;
    _time_stamp = time_stamp;
    _template_name = root["type"].asString();
    const Json::Value world_situation = root["world_situation"];

    if (world_situation) {
        if (world_situation["position"]) {
            _has_position = true;
            _pos(0) = world_situation["position"][0].asDouble();
            _pos(1) = world_situation["position"][1].asDouble();
        }
        if (world_situation["velocity"]) {
            _has_velocity = true;
            _vel(0) = world_situation["velocity"][0].asDouble();
            _vel(1) = world_situation["velocity"][1].asDouble();
        }
        if (world_situation["acceleration"]) {
            _has_acceleration = true;
            _acc(0) = world_situation["acceleration"][0].asDouble();
            _acc(1) = world_situation["acceleration"][1].asDouble();
        }
    }
}

CreationEvent::~CreationEvent() {}

DestructionEvent::DestructionEvent(float& time_stamp, const Json::Value& root)
    : DestructionEvent() {
    _is_destruction = true;
    _time_stamp = time_stamp;
    _entity_type = root["type"].asString();
    _id = root["id"].asInt();
}

DestructionEvent::~DestructionEvent() {}

bool operator<(const std::shared_ptr<WorldEvent>& lhs,
               const std::shared_ptr<WorldEvent>& rhs) {
    float lhs_time = lhs.get()->time_stamp();
    float rhs_time = rhs.get()->time_stamp();

    if (lhs_time != rhs_time) {
        return lhs_time < rhs_time;
    } else {
        return lhs.get() < rhs.get();
    }
}

bool operator<(const std::weak_ptr<WorldEvent>& lhs,
               const std::weak_ptr<WorldEvent>& rhs) {
    return lhs.lock() < rhs.lock();
}
