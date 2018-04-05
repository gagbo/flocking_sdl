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

#include <Eigen/Dense>
#include <memory>
#include <vector>

// A 2 dimensional templated k-d tree
template <typename T>  // T has a get_pos method that returns a Vector
                       // implementing operator()
class KDTree {
 public:
    KDTree() = default;
    ~KDTree() { delete root; }

    inline void insert(const std::shared_ptr<T> x) {
        insert(x, root, KD_DIM_1);
    }

    inline auto get_root() const { return root; }

    inline std::vector<std::weak_ptr<T>> norm1_range_query(const T& center,
                                                           float radius) const {
        return norm1_range_query(center.get_pos(), radius);
    }

    inline std::vector<std::weak_ptr<T>> norm1_range_query(
        const Eigen::Vector2d& center, float radius) const {
        return norm1_range_query(center, radius, root, KD_DIM_1);
    }

    inline void clean() {
        delete root;
        root = nullptr;
    }

 private:
    struct KDNode {
        std::weak_ptr<T> data{};
        KDNode* left{nullptr};
        KDNode* right{nullptr};
        KDNode() = default;
        KDNode(const std::shared_ptr<T>& x) : data(x) {}
        inline auto get_data() { return data; }
        auto x() {
            if (auto spt = data.lock()) {
                return spt->get_pos()(0);
            } else {
                return -1.0;
            }
        }
        auto y() {
            if (auto spt = data.lock()) {
                return spt->get_pos()(1);
            } else {
                return -1.0;
            }
        }
        inline auto go_left() { return left; }
        inline auto go_right() { return right; }
        ~KDNode() {
            delete left;
            delete right;
            data.reset();
        }
    };

    void insert(const std::shared_ptr<T>& x, KDNode*& current,
                int split_direction) {
        if (current == nullptr) {
            current = new KDNode(x);
        } else if (x->get_pos()(split_direction) <
                   current->data.lock()->get_pos()(split_direction)) {
            insert(x, current->left, (split_direction + 1) % KD_TOT_DIM);
        } else {
            insert(x, current->right, (split_direction + 1) % KD_TOT_DIM);
        }
    }

    std::vector<std::weak_ptr<T>> norm1_range_query(
        const Eigen::Vector2d& center, float radius, const KDNode*&& current,
        int split_direction) const {
        std::vector<std::weak_ptr<T>> result;
        float pos_rad = std::abs(radius);

        if (current != nullptr && radius != 0) {
            if (auto spt = current->data.lock()) {
                // We want to find all points so that
                // center(0) - radius <= x <= center(0) + radius
                // center(1) - radius <= y <= center(1) + radius
                double x(spt->get_pos()(0));
                double y(spt->get_pos()(1));
                double min[KD_TOT_DIM] = {center(0) - pos_rad,
                                          center(1) - pos_rad};
                double max[KD_TOT_DIM] = {center(0) + pos_rad,
                                          center(1) + pos_rad};

                // Check if current point is in range
                if (min[0] <= x && x <= max[0] && min[1] <= y && y <= max[1]) {
                    result.push_back(current->data);
                }

                // Recursively check left children if there may be points there
                if (min[split_direction] <= spt->get_pos()(split_direction)) {
                    auto left_result =
                        norm1_range_query(center, radius, current->left,
                                          (split_direction + 1) % KD_TOT_DIM);
                    result.insert(result.end(), left_result.begin(),
                                  left_result.end());
                }

                // Recursively check right children if there may be points there
                if (max[split_direction] >= spt->get_pos()(split_direction)) {
                    auto right_result =
                        norm1_range_query(center, radius, current->right,
                                          (split_direction + 1) % KD_TOT_DIM);
                    result.insert(result.end(), right_result.begin(),
                                  right_result.end());
                }
            }
        }

        return result;
    }

 protected:
    KDNode* root{nullptr};
};

#endif  // WORLD_KDTREE_H_
