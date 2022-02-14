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

#include <atomic>
#include <cstdint>

#include "fmt/format.h"

namespace wdp {
  class Id {
  public:
    using Value = std::uint64_t;

    constexpr explicit Id(Value value) noexcept : value_{value} {}

    /// @throw std::logic_error if all unique Ids are exhausted
    static Id create_unique();

    constexpr bool operator==(const Id&) const noexcept = default;

    constexpr Value value() const noexcept { return value_; }

  private:
    static std::atomic<Value> counter_;

    Value value_;
  };
}

template <>
struct fmt::formatter<wdp::Id> : fmt::formatter<std::string> {
  template <class FC>
  auto format(const wdp::Id& id, FC& ctx) {
    const auto str = fmt::format("{:x}", id.value());
    return fmt::formatter<std::string>::format(str, ctx);
  }
};
