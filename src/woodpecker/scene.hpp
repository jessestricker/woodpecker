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

#pragma once

#include <algorithm>
#include <list>
#include <ranges>
#include <span>
#include <vector>

#include <woodpecker/id.hpp>
#include <woodpecker/joint.hpp>
#include <woodpecker/part.hpp>
#include <woodpecker/pga.hpp>

namespace wdp {
  class Scene {
  public:
    inline static const auto ground_plane = kln::plane{0, 1, 0, 0};  // y = 0
    inline static const auto vertical_axis = kln::origin{} & kln::point{0, 1, 0};

    auto parts() const { return parts_ | std::ranges::views::all; }
    auto parts() { return parts_ | std::ranges::views::all; }

    bool has_part(Id part_id) const;

    Part& add_part();

    /// Removes the part with specified id and all joints connecting this part to other.
    /// Returns the list of removed joints.
    std::vector<Joint> remove_part(Id part_id);

    /// Adds a new joint between two existing parts.
    Joint& add_joint(JointTypePtr type, Id part_id_0, Id part_id_1);

    void remove_joint(Id joint_id);

  private:
    IdPool parts_ids_;
    std::list<Part> parts_;

    IdPool joints_ids_;
    std::list<Joint> joints_;
  };
}
