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

#include <Eigen/Dense>
#include <fstream>
#include <memory>
#include <string>
#include "FlockingConfig.h"
#include "catch.hpp"
#include "ui/input/json_event.h"

TEST_CASE("WorldEventsList construction", "[events][json_read]") {
    Json::Value sample_events;
    std::fstream sample_events_file;
    sample_events_file.open(std::string(DATA_DIR) + "events/event_sample.json",
                            std::ios::in);
    REQUIRE(sample_events_file.is_open());
    WorldEventsList test_list(sample_events_file);

    SECTION("Has the correct number of events") {
        CHECK(test_list.list().size() == 3);
    }
    // TODO: Add a double event (creation + destruction at the same time_stamp)
    // in sample file

    SECTION("Can request correctly the events") {
        auto before_all = test_list.events_in_time_frame(-1, -0.5);
        CHECK(before_all.size() == 0);

        auto before_all_pos = test_list.events_in_time_frame(0, 2);
        CHECK(before_all_pos.size() == 0);

        auto one_event_no_border = test_list.events_in_time_frame(2.4, 2.6);
        CHECK(one_event_no_border.size() == 1);

        auto one_event_test_excl = test_list.events_in_time_frame(2, 3);
        CHECK(one_event_test_excl.size() == 1);

        auto one_event_test_incl = test_list.events_in_time_frame(2.5, 2.8);
        CHECK(one_event_test_incl.size() == 1);

        auto all_events = test_list.events_in_time_frame(2.2, 3.5);
        CHECK(all_events.size() == 3);

        auto two_events = test_list.events_in_time_frame(2.8, 3.2);
        CHECK(two_events.size() == 2);
    }

    SECTION("Parse the correct event type") {
        auto first_event = test_list.events_in_time_frame(2.4, 2.6);
        REQUIRE(first_event.size() == 1);
        CHECK(first_event[0].lock().get()->is_creation());

        auto last_two = test_list.events_in_time_frame(2.9, 3.1);
        auto creation_found = std::find_if(
            last_two.begin(), last_two.end(),
            [&](auto item) { return item.lock().get()->is_creation(); });
        auto destruction_found = std::find_if(
            last_two.begin(), last_two.end(),
            [&](auto item) { return item.lock().get()->is_destruction(); });

        CHECK(creation_found != last_two.end());
        CHECK(destruction_found != last_two.end());
    }
}
