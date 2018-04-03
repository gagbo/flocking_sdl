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
        REQUIRE(tree.get_root()->get_data() == ent_1);
        REQUIRE(tree.get_root()->go_left()->get_data() == ent_2);
        REQUIRE(tree.get_root()->go_right()->get_data() == ent_3);
        REQUIRE(tree.get_root()->go_right()->go_left()->get_data() == ent_4);
        REQUIRE(tree.get_root()->go_right()->go_right()->get_data() == ent_5);
    }

    SECTION("Test the data accessors from the tree") {
        REQUIRE(tree.get_root()->x() == Approx(ent_1->get_pos()(0)));
        REQUIRE(tree.get_root()->y() == Approx(ent_1->get_pos()(1)));
    }
}
