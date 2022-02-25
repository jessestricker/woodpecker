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

#include "joint.hpp"

#include <woodpecker/util/assert.hpp>

namespace wdp {
  Joint::Joint(Id id, JointTypePtr type, const Joint::PartIds& part_ids)
      : id(id), type(std::move(type)), part_ids(part_ids) {
    WDP_ASSERT(id);
  }
}
