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

#include <woodpecker/joint.hpp>
#include <woodpecker/part.hpp>
#include <woodpecker/pga.hpp>
#include <woodpecker/util/assert.hpp>

namespace wdp {
  class Scene {
  public:
    inline static const auto ground_plane = kln::plane{1, 0, 1, 0};
    inline static const auto vertical_axis = kln::origin{} & kln::point{0, 1, 0};

    auto parts() const { return parts_ | std::ranges::views::all; }
    auto parts() { return parts_ | std::ranges::views::all; }

    Part& add_part() { return parts_.emplace_back(); }

    void remove_part(const Part* part) {
      WDP_ASSERT(part != nullptr);
      // TODO: remove joints with this part
      parts_.remove_if([&](const auto& elem) { return &elem == part; });
    }

    void add_joint();
    void remove_joint();

  private:
    std::list<Part> parts_;
    std::vector<Joint> joints_;
  };
}
