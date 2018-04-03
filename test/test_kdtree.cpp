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
#include "entity/entity.h"
#include "entity/ant/ant.h"
#include "entity/food/food.h"
#include "world/world.h"
#include "world/kdtree.h"

TEST_CASE("Insertion into a tree", "[kdtree][insert]") {
    KDTree<Entity> tree;
    World dummy;
    SECTION("Test the insertions are done in the correct position") {
        std::shared_ptr<Entity> ent_1(
            std::move(Entity::makeEntity(Entity::Type::ANT, 1, dummy, 50.0, 50.1)));
        std::shared_ptr<Entity> ent_2(
            std::move(Entity::makeEntity(Entity::Type::ANT, 2, dummy, 25.0, 55.2)));
        std::shared_ptr<Entity> ent_3(
            std::move(Entity::makeEntity(Entity::Type::ANT, 3, dummy, 75.0, 22.3)));
        std::shared_ptr<Entity> ent_4(
            std::move(Entity::makeEntity(Entity::Type::ANT, 4, dummy, 60.0, 21.0)));

        tree.insert(ent_1);
        tree.insert(ent_2);
        tree.insert(ent_3);
        tree.insert(ent_4);
        // root should be ent_1
        REQUIRE(tree.get_root()->get_data() == ent_1);
        REQUIRE(tree.get_root()->x() == Approx(ent_1->get_pos()(0)));
        REQUIRE(tree.get_root()->y() == Approx(ent_1->get_pos()(1)));

        REQUIRE(tree.get_root()->go_left()->get_data() == ent_2);
        REQUIRE(tree.get_root()->go_right()->get_data() == ent_3);
        REQUIRE(tree.get_root()->go_right()->go_left()->get_data() == ent_4);
    }
}
