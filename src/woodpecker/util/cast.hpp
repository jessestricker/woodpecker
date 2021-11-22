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

#include <stdexcept>
#include <type_traits>
#include <utility>

namespace wdp {
  class NarrowOutOfRange : public std::logic_error {
  public:
    NarrowOutOfRange() : logic_error("narrowing conversion was out of range") {
    }
  };

  /// Converts a value from one arithmetic type to another,
  /// throwing if the value can not be represented in the target type.
  template <class To, class From>
  auto narrow(From from) {
    static_assert(std::is_arithmetic_v<To> && std::is_arithmetic_v<From>, "To and From must be arithmetic types");
    const auto to_value = static_cast<To>(from);
    // check casting back
    if (static_cast<From>(to_value) == from) {
      return to_value;
    }
    throw NarrowOutOfRange{};
  }
}
