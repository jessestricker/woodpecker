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

#include <cstdlib>
#include <string_view>
#include <source_location>

#include <spdlog/spdlog.h>

namespace wdp {
  struct SourceLocation {
    std::string_view file;
    unsigned line{};
    std::string_view function;  // optional, may be empty
  };
}

template <>
struct fmt::formatter<wdp::SourceLocation> : fmt::formatter<std::string_view> {
  template <class FormatContext>
  auto format(const wdp::SourceLocation& src_loc, FormatContext& ctx) {
    auto out = fmt::format_to(ctx.out(), "{}:{}",src_loc.file, src_loc.line);
    if (!src_loc.function.empty()) {
      out = fmt::format_to(out, ", {}", src_loc.function);
    }
    return out;
  }
};

#if defined(_MSC_VER)
#define WDP_PRETTY_FUNCTION __FUNCSIG__
#elif defined(__clang__) || defined(__GNUC__)
#define WDP_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
#error unsupported compiler
#endif

#define WDP_CURRENT_SOURCE_LOCATION \
  ::wdp::SourceLocation { __FILE__, __LINE__, WDP_PRETTY_FUNCTION }

namespace wdp {
  class Assertion {
  public:
    Assertion(std::string_view expr_str, const SourceLocation& src_loc, std::string_view message = "")
        : expr_str_{expr_str}, src_loc_{src_loc}, message_{message} {}

    template <class ExprLambda>
    void operator()(const ExprLambda& expr_lambda) noexcept {
      const auto expr_value = expr_lambda();
      if (expr_value) [[likely]] {
        return;
      }
      log_violation();
      std::abort();
    }

  private:
    std::string_view expr_str_;
    SourceLocation src_loc_;
    std::string_view message_;  // optional, may be empty

    void log_violation() noexcept {
      spdlog::critical("assertion violated: {}", expr_str_);
      spdlog::critical("  source: {}", src_loc_);
      if (!message_.empty()) {
        spdlog::critical("  message: {}", message_);
      }
      spdlog::shutdown();
    }
  };
}

#define WDP_ASSERT(expr__, ...) \
  ::wdp::Assertion{#expr__, WDP_CURRENT_SOURCE_LOCATION, __VA_ARGS__}([&]() noexcept { return expr__; })
