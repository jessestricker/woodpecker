// Copyright © 2021 Jesse Stricker
//
// This file is part of Woodpecker.
//
// Woodpecker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Woodpecker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Woodpecker.  If not, see <https://www.gnu.org/licenses/>.

#include "scene.hpp"

#include <woodpecker/util/assert.hpp>

namespace wdp {
  bool Scene::has_part(Id part_id) const {
    return std::ranges::any_of(parts_, [&](const auto& part) { return part.id() == part_id; });
  }

  Part& Scene::add_part() { return parts_.emplace_back(parts_ids_()); }

  std::vector<Joint> Scene::remove_part(Id part_id) {
    auto removed_joints = std::vector<Joint>{};
    // remove joints
    for (auto it = joints_.begin(); it != joints_.end();) {
      if (std::ranges::any_of(it->part_ids, [&](const auto& item) { return item == part_id; })) {
        removed_joints.push_back(std::move(*it));
        it = joints_.erase(it);
      } else {
        ++it;
      }
    }
    // remove part
    parts_.remove_if([&](const auto& part) { return part.id() == part_id; });
    return removed_joints;
  }

  Joint& Scene::add_joint(JointTypePtr type, Id part_id_0, Id part_id_1) {
    WDP_ASSERT(type != nullptr);
    WDP_ASSERT(has_part(part_id_0));
    WDP_ASSERT(has_part(part_id_1));

    return joints_.emplace_back(joints_ids_(), std::move(type), Joint::PartIds{part_id_0, part_id_1});
  }

  void Scene::remove_joint(Id joint_id) {
    joints_.remove_if([&](const auto& joint) { return joint.id == joint_id; });
  }
}
