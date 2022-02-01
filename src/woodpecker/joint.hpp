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
  enum class Fastener {
    // mechanical
    dowel,
    screw,
    nail,
    // other
    glue
  };

  class JointTypeBase {
  public:
    virtual ~JointTypeBase() noexcept = default;
  };

  struct MiterJointType : JointTypeBase {};

  struct ButtJointType : JointTypeBase {
    Fastener fastener;
  };

  struct Joint {
    std::unique_ptr<JointTypeBase> type;
    std::array<const Part*, 2> parts;
  };
}
