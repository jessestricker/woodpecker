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

#include <cmath>
#include <numbers>

#include <klein/klein.hpp>

namespace wdp {
  /// The Archimedes constant as a single-precision floating-point number.
  inline constexpr auto pi = std::numbers::pi_v<float>;

  /// A motor that when applies does not change the argument.
  inline static const auto identity_motor = kln::motor{1, 0, 0, 0, 0, 0, 0, 0};

  /// This namespace contains fixed versions of broken functions in the `kln` namespace.
  namespace fix_kln {
    inline auto normalized(const kln::plane& p) noexcept { return p / p.norm(); }

    inline auto normalized(const kln::line& l) noexcept { return l / std::hypot(l.e12(), l.e31(), l.e23()); }
  }
}
