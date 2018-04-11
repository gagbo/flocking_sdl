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
#include <string>
#include <memory>
#include "catch.hpp"
#include "entity/ant/ant.h"
#include "jsoncpp/json/json.h"
#include "world/world.h"
#include "FlockingConfig.h"

TEST_CASE("World setting functions", "[world][set]") {
    World world;
    SECTION("Set world size") {
        world.set_world_size(420, 122);
        REQUIRE(world.width == Approx(420));
        REQUIRE(world.height == Approx(122));
    }

    SECTION("Set world time step") {
        world.set_time_step(1e-5);
        REQUIRE(world.time_step == Approx(1e-5));
    }
}

TEST_CASE("World wrapping functions", "[world][wrap]") {
    World world(640, 480, 1e-2);
    Eigen::Vector2d pos1;
    Eigen::Vector2d pos2;
    Eigen::Vector2d pos3;
    SECTION("wrap around --- do nothing") {
        pos1 << 24, 100;
        world.wrap_around(pos1);
        REQUIRE(pos1(0) == 24);
        REQUIRE(pos1(1) == 100);
    }
    SECTION("wrap around --- over in width") {
        pos1 << 748, 54;
        world.wrap_around(pos1);
        REQUIRE(pos1(0) == 108);
        REQUIRE(pos1(1) == 54);
    }
    SECTION("wrap around --- under in width") {
        pos1 << -12, 400;
        world.wrap_around(pos1);
        REQUIRE(pos1(0) == 628);
        REQUIRE(pos1(1) == 400);
    }
    SECTION("wrap around --- over in height") {
        pos1 << 89, 800;
        world.wrap_around(pos1);
        REQUIRE(pos1(0) == 89);
        REQUIRE(pos1(1) == 320);
    }
    SECTION("wrap around --- under in height") {
        pos1 << 490, -461;
        world.wrap_around(pos1);
        REQUIRE(pos1(0) == 490);
        REQUIRE(pos1(1) == 19);
    }
    SECTION("wrap around --- multiple passes") {
        pos1 << -3542, 1042;
        world.wrap_around(pos1);
        REQUIRE(pos1(0) == 298);
        REQUIRE(pos1(1) == 82);
    }

    SECTION("point to --- do nothing") {
        pos1 << 23, 400;
        pos2 << 150, 300;
        pos3 = world.point_to(pos1, pos2);

        REQUIRE(pos3(0) == 127);
        REQUIRE(pos3(1) == -100);
    }
    SECTION("point to --- go through left") {
        pos1 << 23, 400;
        pos2 << 630, 300;
        pos3 = world.point_to(pos1, pos2);

        REQUIRE(pos3(0) == -33);
        REQUIRE(pos3(1) == -100);
    }
    SECTION("point to --- go through right") {
        pos1 << 630, 400;
        pos2 << 10, 300;
        pos3 = world.point_to(pos1, pos2);

        REQUIRE(pos3(0) == 20);
        REQUIRE(pos3(1) == -100);
    }
    SECTION("point to --- go through up") {
        pos1 << 23, 24;
        pos2 << 150, 370;
        pos3 = world.point_to(pos1, pos2);

        REQUIRE(pos3(0) == 127);
        REQUIRE(pos3(1) == -134);
    }
    SECTION("point to --- go through down") {
        pos1 << 23, 400;
        pos2 << 150, 10;
        pos3 = world.point_to(pos1, pos2);

        REQUIRE(pos3(0) == 127);
        REQUIRE(pos3(1) == 90);
    }
}

TEST_CASE("World convert functions", "[world][convert]") {
    World world(640, 480, 1e-2);
    Eigen::Vector2d pos1(0, 0);
    Eigen::Vector2d pos2(639, 479);
    Eigen::Vector2d pos3(323, 197);
    Eigen::Vector2d res;

    SECTION("Convert -- unchanged scale") {
        world.width_in_px = world.width;
        world.height_in_px = world.height;
        res = world.convert(pos1);
        REQUIRE(res(0) == 0);
        REQUIRE(res(1) == 0);
        res = world.convert(pos2);
        REQUIRE(res(0) == 639);
        REQUIRE(res(1) == 479);
        res = world.convert(pos3);
        REQUIRE(res(0) == 323);
        REQUIRE(res(1) == 197);
    }

    SECTION("Convert -- reduced scale") {
        world.width_in_px = 67;
        world.height_in_px = 95;
        res = world.convert(pos1);
        REQUIRE(res(0) == 0);
        REQUIRE(res(1) == 0);
        res = world.convert(pos2);
        REQUIRE(res(0) == world.width_in_px - 1);
        REQUIRE(res(1) == world.height_in_px - 1);
        res = world.convert(pos3);
        REQUIRE(res(0) ==
                std::floor(pos3(0) / world.width * world.width_in_px));
        REQUIRE(res(1) ==
                std::floor(pos3(1) / world.height * world.height_in_px));
    }

    SECTION("Convert -- bigger scale") {
        world.width_in_px = 1000;
        world.height_in_px = 1000;
        res = world.convert(pos1);
        REQUIRE(res(0) == 0);
        REQUIRE(res(1) == 0);
        res = world.convert(pos2);
        REQUIRE(res(0) ==
                std::floor(pos2(0) / world.width * world.width_in_px));
        REQUIRE(res(1) ==
                std::floor(pos2(1) / world.height * world.height_in_px));
        res = world.convert(pos3);
        REQUIRE(res(0) ==
                std::floor(pos3(0) / world.width * world.width_in_px));
        REQUIRE(res(1) ==
                std::floor(pos3(1) / world.height * world.height_in_px));
    }
}

TEST_CASE("World adds entities", "[world][add_entity]") {
    World world(640, 480, 1e-2);

    SECTION("Add random ant") {
        world.add_entity(Entity::Type::ANT);
        REQUIRE(world.get_entity_list().size() == 1);
        auto it = world.get_entity_list().begin();
        Eigen::Vector2d pos = (*it)->get_pos();
        REQUIRE(pos[0] < world.width);
        REQUIRE(pos[0] >= 0);
        REQUIRE(pos[1] < world.height);
        REQUIRE(pos[1] >= 0);
    }

    SECTION("Add positioned ant") {
        world.add_entity(Entity::Type::ANT, 23.9, 90);
        REQUIRE(world.get_entity_list().size() == 1);
        auto it = world.get_entity_list().begin();
        Eigen::Vector2d pos = (*it)->get_pos();
        REQUIRE(pos[0] == Approx(23.9));
        REQUIRE(pos[1] == Approx(90));
    }

    SECTION("Add random food") {
        world.add_entity(Entity::Type::FOOD);
        REQUIRE(world.get_entity_list().size() == 1);
        auto it = world.get_entity_list().begin();
        Eigen::Vector2d pos = (*it)->get_pos();
        REQUIRE(pos[0] < world.width);
        REQUIRE(pos[0] >= 0);
        REQUIRE(pos[1] < world.height);
        REQUIRE(pos[1] >= 0);
    }

    SECTION("Add positioned food") {
        world.add_entity(Entity::Type::FOOD, 23.9, 90);
        REQUIRE(world.get_entity_list().size() == 1);
        auto it = world.get_entity_list().begin();
        Eigen::Vector2d pos = (*it)->get_pos();
        REQUIRE(pos[0] == Approx(23.9));
        REQUIRE(pos[1] == Approx(90));
    }

    SECTION("Add multiple entities") {
        world.add_entity(Entity::Type::FOOD);
        world.add_entity(Entity::Type::ANT);
        world.add_entity(Entity::Type::ANT);
        world.add_entity(Entity::Type::FOOD);
        REQUIRE(world.get_entity_list().size() == 4);
        auto it = world.get_entity_list().begin();
        REQUIRE((*it)->get_type_string() == "Food");
        it = std::next(it);
        REQUIRE((*it)->get_type_string() == "Ant");
        it = std::next(it);
        REQUIRE((*it)->get_type_string() == "Ant");
        it = std::next(it);
        REQUIRE((*it)->get_type_string() == "Food");
    }

    SECTION("Add json ant") {
        Json::Value soldier_root;
        std::fstream soldier_file;
        soldier_file.open(std::string(DATA_DIR) + "entity/ant_soldier.json", std::ios::in);
        REQUIRE(soldier_file.is_open());

        soldier_file >> soldier_root;
        world.add_entity("ant_soldier.json");
        REQUIRE(world.get_entity_list().size() == 1);
        auto it = world.get_entity_list().begin();
        auto pAnt = std::dynamic_pointer_cast<Ant>(*it);
        CHECK(pAnt->get_cruise_speed() == Approx(soldier_root["cruise_speed"].asFloat()));
    }

    SECTION("Add positionned json ant") {
        Json::Value worker_root;
        std::fstream worker_file;
        worker_file.open(std::string(DATA_DIR) + "entity/ant_worker.json", std::ios::in);
        REQUIRE(worker_file.is_open());

        worker_file >> worker_root;
        world.add_entity("ant_worker.json", 23.9, 90);
        REQUIRE(world.get_entity_list().size() == 1);
        auto it = world.get_entity_list().begin();
        auto pAnt = std::dynamic_pointer_cast<Ant>(*it);
        Eigen::Vector2d pos = pAnt->get_pos();
        CHECK(pos[0] == Approx(23.9));
        CHECK(pos[1] == Approx(90));
        CHECK(pAnt->get_cruise_speed() == Approx(worker_root["cruise_speed"].asFloat()));
    }
}

TEST_CASE("World computes good neighbourhoods for entities",
          "[world][neighbour_computation]") {
    World world(640, 480, 1e-2);
    Ant* inside_see_all = dynamic_cast<Ant*>(
        world.add_entity(Entity::Type::ANT, 320.0, 240.0).lock().get());
    inside_see_all->set_vision_distance(340.0);
    Ant* corner_see_all = dynamic_cast<Ant*>(
        world.add_entity(Entity::Type::ANT, 40.0, 40.0).lock().get());
    corner_see_all->set_vision_distance(600.0);
    Ant* corner_2 = dynamic_cast<Ant*>(
        world.add_entity(Entity::Type::ANT, 600.0, 40.0).lock().get());
    corner_2->set_vision_distance(90.0);
    Ant* corner_3 = dynamic_cast<Ant*>(
        world.add_entity(Entity::Type::ANT, 600.0, 440.0).lock().get());
    corner_3->set_vision_distance(90.0);
    Ant* corner_4 = dynamic_cast<Ant*>(
        world.add_entity(Entity::Type::ANT, 40.0, 440.0).lock().get());
    corner_4->set_vision_distance(90.0);

    world.update_tree();
    world.update_entity_neighbourhoods();
    SECTION("Entities are seen only once") {
        auto inside_neigh = inside_see_all->get_neighbourhood();
        CHECK(inside_neigh.size() == 5);
        inside_see_all->set_vision_distance(8000.0);
        world.update_tree();
        world.update_entity_neighbourhoods();  // Invalidates inside_neigh
        inside_neigh = inside_see_all->get_neighbourhood();
        CHECK(inside_neigh.size() == 5);
    }
}
