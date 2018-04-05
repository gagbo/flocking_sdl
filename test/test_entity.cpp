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

TEST_CASE("Ant vision", "[world][vision]") {
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
