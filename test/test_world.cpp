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

#include "catch.hpp"
#include "world/world.h"
#include <Eigen/Dense>

TEST_CASE("World setting functions", "[world][set]") {
  World world(34, 32, 1e-2);
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