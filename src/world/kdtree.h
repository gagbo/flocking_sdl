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

#ifndef WORLD_KDTREE_H_
#define WORLD_KDTREE_H_

#define KD_TOT_DIM 2
#define KD_DIM_1 0
#define KD_DIM_2 1

#include <memory>
#include <vector>

// A 2 dimensional templated k-d tree
template <typename T>  // T has a get_pos method that returns a Vector
                       // implementing operator()
class KDTree {
 public:
    KDTree() = default;

    inline void insert(const std::shared_ptr<T> x) {
        insert(x, root, KD_DIM_1);
    }

    inline auto get_root() const { return root; }

    inline std::vector<std::shared_ptr<T>> norm1_range_query(
        const T& center, float radius) const {
        return norm1_range_query(center, radius, root, KD_DIM_1);
    }

 private:
    struct KDNode {
        std::shared_ptr<T> data{};
        KDNode* left{nullptr};
        KDNode* right{nullptr};
        KDNode() = default;
        KDNode(const std::shared_ptr<T>& x) { data = x; }
        inline auto get_data() { return data; }
        inline auto x() { return data->get_pos()(0); }
        inline auto y() { return data->get_pos()(1); }
        inline auto go_left() { return left; }
        inline auto go_right() { return right; }
    };

    void insert(const std::shared_ptr<T>& x, KDNode*& current,
                int split_direction) {
        if (current == nullptr) {
            current = new KDNode(x);
        } else if (x->get_pos()(split_direction) <
                   current->data->get_pos()(split_direction)) {
            insert(x, current->left, (split_direction + 1) % KD_TOT_DIM);
        } else {
            insert(x, current->right, (split_direction + 1) % KD_TOT_DIM);
        }
    }

    std::vector<std::shared_ptr<T>> norm1_range_query(
        const T& center, float radius, const KDNode*&& current,
        int split_direction) const {
        std::vector<std::shared_ptr<T>> result;
        return result;
    }

 protected:
    KDNode* root{nullptr};
};

#endif  // WORLD_KDTREE_H_
