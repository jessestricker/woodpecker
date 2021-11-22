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

// fix std::source_location not being found by CLion
#ifndef __cpp_consteval
#define __cpp_consteval
#endif

#include <source_location>
#include <stdexcept>
#include <string_view>

#include <spdlog/fmt/fmt.h>

namespace wdp {
  class AssertionViolation : public std::logic_error {
  public:
    using std::logic_error::logic_error;
  };

  namespace details {
    template <class Fn>
    void do_assert(const Fn& expr_lambda, std::string_view expr_str, std::source_location src_loc,
                   std::string_view message = "") {
      const auto succeeded = expr_lambda();
      if (!succeeded) [[unlikely]] {
        auto what_text =
            fmt::format("assertion violated: {}\nsource: {}:{}", expr_str, src_loc.file_name(), src_loc.line());
        if (!message.empty()) {
          what_text += "\nmessage: ";
          what_text += message;
        }
        throw AssertionViolation{what_text};
      }
    }
  }
}

#define WDP_ASSERT(expr__, ...) \
  ::wdp::details::do_assert([&]() noexcept { return expr__; }, #expr__, std::source_location::current(), __VA_ARGS__)
