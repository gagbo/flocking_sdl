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
#include "catch.hpp"
#include "entity/ant/ant.h"
#include "entity/entity.h"
#include "entity/food/food.h"
#include "world/world.h"

TEST_CASE("Ant setters", "[ant][set]") {
    World world(640, 480, 1);
    Ant ant_1(1, world, 50.0, 31.0, 0.8, -0.5, 0, 0);
    ant_1.set_vision_distance(10.0);
    ant_1.set_vision_angle_deg(45.0);
    ant_1.set_size(8, 7.9);
    ant_1.set_mass(2.3);
    ant_1.set_max_acceleration(25.7);
    ant_1.set_cruise_speed(20);
    CHECK(ant_1.get_color()[0] == ant_1.default_color[0]);
    CHECK(ant_1.get_color()[1] == ant_1.default_color[1]);
    CHECK(ant_1.get_color()[2] == ant_1.default_color[2]);
    CHECK(ant_1.get_color()[3] == ant_1.default_color[3]);

    CHECK(ant_1.get_size()(0) == Approx(8));
    CHECK(ant_1.get_size()(1) == Approx(7.9));

    CHECK(ant_1.get_mass() == Approx(2.3));
    CHECK(ant_1.get_max_acceleration() == Approx(25.7));
    CHECK(ant_1.get_cruise_speed() == Approx(20));
}

TEST_CASE("Ant vision", "[ant][vision]") {
    World world(640, 480, 1);
    Ant ant_1(1, world, 50.0, 31.0, 0.8, -0.5, 0, 0);
    ant_1.set_vision_distance(10.0);
    ant_1.set_vision_angle_deg(45.0);
    Food food_1(1, world);
    Food food_2(2, world);
    Food food_3(3, world);
    Food food_4(4, world);

    SECTION("Check that vision is properly set") {
        Eigen::Vector2d self(0, 0);
        CHECK(ant_1.is_in_vision_triangle(self) == true);

        Eigen::Vector2d too_far(88, -55);
        CHECK(ant_1.is_in_vision_triangle(too_far) == false);

        Eigen::Vector2d on_axis(1.6, -1.0);
        CHECK(ant_1.is_in_vision_triangle(on_axis) == true);

        Eigen::Vector2d on_axis_behind(-1.6, 1.0);
        CHECK(ant_1.is_in_vision_triangle(on_axis) == true);

        Eigen::Vector2d in_angle(1.4, -1.0);
        CHECK(ant_1.is_in_vision_triangle(in_angle) == true);

        Eigen::Vector2d out_of_angle(-1, 0.2);
        CHECK(ant_1.is_in_vision_triangle(out_of_angle) == false);
    }
}

TEST_CASE("Ant decision", "[ant][decision]") {
    World world(640, 480, 1);
    Ant* ant_1 = dynamic_cast<Ant*>(
        world.add_entity(Entity::Type::ANT, 50.0, 31.0, 1.0, 1.0).lock().get());
    Food* dummy = dynamic_cast<Food*>(
        world.add_entity(Entity::Type::FOOD, 54.0, 35.0).lock().get());
    ant_1->set_vision_distance(10.0);
    ant_1->set_vision_angle_deg(45.0);
    ant_1->set_max_acceleration(1e20);
    ant_1->set_friction_factor(0);
    ant_1->set_cruise_speed(2);

    REQUIRE(world.get_time_step() == Approx(1));
    REQUIRE(ant_1->get_max_acceleration() == Approx(1e20));

    // SECTION("Test Separation") {
    //   ant_1->set_separation_weight(1);
    //   ant_1->set_alignment_weight(0);
    //   ant_1->set_cohesion_weight(0);
    //   ant_1->set_separation_exp(0.5);
    //   world.update();
    //   REQUIRE(true);
    // }

    SECTION("Test Alignment") {
        ant_1->set_separation_weight(0);
        ant_1->set_alignment_weight(1);
        ant_1->set_cohesion_weight(0);
        world.update_tree();
        world.update_entity_neighbourhoods();
        REQUIRE(ant_1->decision_alignment_velocity()(0) ==
                Approx(std::sqrt(2)));
        REQUIRE(ant_1->decision_alignment_velocity()(1) ==
                Approx(std::sqrt(2)));
        world.call_entity_decision();
        world.update_entity_and_renderer();
        REQUIRE(ant_1->get_neighbourhood().size() == 2);
        CHECK(ant_1->get_vel()(0) == Approx(std::sqrt(2)));
        CHECK(ant_1->get_vel()(1) == Approx(std::sqrt(2)));
    }

    SECTION("Test Cohesion") {
        ant_1->set_separation_weight(0);
        ant_1->set_alignment_weight(0);
        ant_1->set_cohesion_weight(1);
        world.update_tree();
        world.update_entity_neighbourhoods();
        REQUIRE(ant_1->decision_cohesion_velocity()(0) ==
                Approx(std::sqrt(2)));
        REQUIRE(ant_1->decision_cohesion_velocity()(1) ==
                Approx(std::sqrt(2)));
        world.call_entity_decision();
        world.update_entity_and_renderer();
        REQUIRE(ant_1->get_neighbourhood().size() == 2);
        CHECK(ant_1->get_pos()(0) == Approx(50 + std::sqrt(2)));
        CHECK(ant_1->get_pos()(1) == Approx(31 + std::sqrt(2)));
    }
}
