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
#include <algorithm>
#include <memory>
#include "catch.hpp"
#include "entity/ant/ant.h"
#include "entity/entity.h"
#include "entity/food/food.h"
#include "world/kdtree.h"

template class KDTree<Entity>;

TEST_CASE("Insertion into a tree", "[kdtree][insert]") {
    KDTree<Entity> tree;
    std::shared_ptr<Entity> ent_1(
        std::move(Entity::makeEntity(Entity::Type::ANT, 50.0, 50.1)));
    std::shared_ptr<Entity> ent_2(
        std::move(Entity::makeEntity(Entity::Type::ANT, 25.0, 55.2)));
    std::shared_ptr<Entity> ent_3(
        std::move(Entity::makeEntity(Entity::Type::ANT, 75.0, 22.3)));
    std::shared_ptr<Entity> ent_4(
        std::move(Entity::makeEntity(Entity::Type::ANT, 60.0, 21.0)));
    std::shared_ptr<Entity> ent_5(
        std::move(Entity::makeEntity(Entity::Type::ANT, 58.75, 37.3)));
    tree.insert(ent_1);

    SECTION("Test the insertions are done in the correct position") {
        tree.insert(ent_2);
        tree.insert(ent_3);
        tree.insert(ent_4);
        tree.insert(ent_5);
        // root should be ent_1
        REQUIRE(tree.root()->data().lock() == ent_1);
        REQUIRE(tree.root()->go_left()->data().lock() == ent_2);
        REQUIRE(tree.root()->go_right()->data().lock() == ent_3);
        REQUIRE(tree.root()->go_right()->go_left()->data().lock() ==
                ent_4);
        REQUIRE(tree.root()->go_right()->go_right()->data().lock() ==
                ent_5);
    }

    SECTION("Test the data accessors from the tree") {
        REQUIRE(tree.root()->x() == Approx(ent_1->pos()(0)));
        REQUIRE(tree.root()->y() == Approx(ent_1->pos()(1)));
    }
}

TEST_CASE("Tree cleanup", "[kdtree][clean]") {
    KDTree<Entity> tree;
    std::shared_ptr<Entity> ent_1(
        std::move(Entity::makeEntity(Entity::Type::ANT, 50.0, 50.1)));
    tree.insert(ent_1);

    SECTION("Correct cleaning up of k-d tree") {
      REQUIRE(tree.root() != nullptr);
      tree.clean();
      REQUIRE(tree.root() == nullptr);
    }
}

TEST_CASE("Range Query in the kdtree", "[kdtree][range_query]") {
    KDTree<Entity> tree;
    std::shared_ptr<Entity> ent_1(
        std::move(Entity::makeEntity(Entity::Type::ANT, 50.0, 50.1)));
    std::shared_ptr<Entity> ent_2(
        std::move(Entity::makeEntity(Entity::Type::FOOD, 25.0, 55.2)));
    std::shared_ptr<Entity> ent_3(
        std::move(Entity::makeEntity(Entity::Type::ANT, 75.0, 22.3)));
    std::shared_ptr<Entity> ent_4(
        std::move(Entity::makeEntity(Entity::Type::FOOD, 60.0, 21.0)));
    std::shared_ptr<Entity> ent_5(
        std::move(Entity::makeEntity(Entity::Type::ANT, 58.75, 37.3)));
    std::shared_ptr<Entity> ent_6(
        std::move(Entity::makeEntity(Entity::Type::FOOD, 19.89, 13.9)));
    std::shared_ptr<Entity> ent_7(
        std::move(Entity::makeEntity(Entity::Type::ANT, 3.1, 67.2)));
    std::shared_ptr<Entity> ent_8(
        std::move(Entity::makeEntity(Entity::Type::FOOD, 75.25, 1.2)));
    std::shared_ptr<Entity> ent_9(
        std::move(Entity::makeEntity(Entity::Type::ANT, 24.98, 39.0)));
    tree.insert(ent_1);
    tree.insert(ent_2);
    tree.insert(ent_3);
    tree.insert(ent_4);
    tree.insert(ent_5);
    tree.insert(ent_6);
    tree.insert(ent_7);
    tree.insert(ent_8);
    tree.insert(ent_9);

    SECTION("Test a successful range query on an entity") {
        Eigen::Vector2d center(60.0, 21.0);
        float radius = 15.0;
        auto result = tree.norm1_range_query(center, radius);

        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_1;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_2;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_3;
                             }) != result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_4;
                             }) != result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_5;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_6;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_7;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_8;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_9;
                             }) == result.end());
    }

    SECTION("Test a successful range query calling from an entity") {
        float radius = 1.0;
        auto result = tree.norm1_range_query(*ent_7.get(), radius);

        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_1;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_2;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_3;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_4;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_5;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_6;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_7;
                             }) != result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_8;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_9;
                             }) == result.end());
    }

    SECTION("Test a successful range query not on entity") {
        Eigen::Vector2d center(10.0, 10.0);
        float radius = 30;
        auto result = tree.norm1_range_query(center, radius);

        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_1;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_2;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_3;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_4;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_5;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_6;
                             }) != result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_7;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_8;
                             }) == result.end());
        REQUIRE(std::find_if(result.begin(), result.end(),
                             [&](const auto& arg) -> bool {
                                 return arg.lock() == ent_9;
                             }) != result.end());
    }

    SECTION("Test an empty range query") {
        Eigen::Vector2d center(20.0, 20.0);
        float radius = 5.9;  // Closest point is ent_6
        auto result = tree.norm1_range_query(center, radius);

        REQUIRE(result.size() == 0);
    }
}
