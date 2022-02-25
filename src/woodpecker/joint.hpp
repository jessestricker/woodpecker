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

#include <array>
#include <memory>

#include <woodpecker/part.hpp>

namespace wdp {
  /// A fastener is used in joints which are not strong enough on their own.
  enum class Fastener {
    // mechanical
    dowel,
    screw,
    nail,
    // other
    glue
  };

  /// The abstract base class of all joint types.
  class JointTypeBase {
  public:
    virtual ~JointTypeBase() noexcept = default;

  protected:
    JointTypeBase() noexcept = default;
  };

  using JointTypePtr = std::unique_ptr<JointTypeBase>;

  struct ButtJointType : JointTypeBase {
    Fastener fastener{Fastener::dowel};
  };

  struct BoxJointType : JointTypeBase {
    // TODO: parameters, like finger width, finger length
  };

  /// A joint is a connection between two parts.
  struct Joint {
    using PartIds = std::array<Id, 2>;

    Id id;
    JointTypePtr type;
    PartIds part_ids;

    Joint(Id id, JointTypePtr type, const PartIds& part_ids);
  };
}
