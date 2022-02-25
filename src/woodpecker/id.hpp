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
#include <mutex>
#include <string>

#include <fmt/format.h>

namespace wdp {
  class Id {
  public:
    using Value = std::uint64_t;

    static constexpr Id invalid() noexcept { return Id{0}; }

    constexpr explicit Id(Value value) noexcept : value_{value} {}
    constexpr explicit operator Value() const noexcept { return value_; }
    friend constexpr bool operator==(Id, Id) noexcept = default;

    constexpr operator bool() const noexcept { return *this != invalid(); }

  private:
    Value value_;
  };

  class IdPool {
  public:
    /// Creates a new unique Id.
    Id operator()();

  private:
    std::mutex mutex_;
    bool exhausted_{false};
    Id::Value last_value_{static_cast<Id::Value>(Id::invalid())};
  };
}

template <>
struct fmt::formatter<wdp::Id> : fmt::formatter<std::string> {
  template <typename FormatContext>
  auto format(const wdp::Id& id, FormatContext& ctx) {
    const auto str = fmt::format("{:016x}", static_cast<wdp::Id::Value>(id));
    return fmt::formatter<std::string>::format(str, ctx);
  }
};
