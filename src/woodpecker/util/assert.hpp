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

#include <spdlog/spdlog.h>

namespace wdp {
  struct SourceLocation {
    const char* file = nullptr;
    unsigned line = 0;
    const char* function = nullptr;  // optional, may be nullptr
  };
}

template <>
struct fmt::formatter<wdp::SourceLocation> : fmt::formatter<string_view> {
  template <class FormatContext>
  auto format(const wdp::SourceLocation& src_loc, FormatContext& ctx) {
    if (src_loc.function != nullptr) {
      return format_to(ctx.out(), "{}:{}, {}", src_loc.file, src_loc.line, src_loc.function);
    }
    return format_to(ctx.out(), "{}:{}", src_loc.file, src_loc.line);
  }
};

#if defined(_MSC_VER)
#define WDP_PRETTY_FUNCTION __FUNCSIG__
#elif defined(__clang__) || defined(__GNUC__)
#define WDP_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
#error unsupported compiler
#endif

#define WDP_CURRENT_SOURCE_LOCATION         \
  ::wdp::SourceLocation {                   \
    __FILE__, __LINE__, WDP_PRETTY_FUNCTION \
  }

namespace wdp {
  class Assertion {
  public:
    Assertion(const char* expr_str, const SourceLocation& src_loc, const char* message = nullptr)
        : expr_str_{expr_str}, src_loc_{src_loc}, message_{message} {
    }

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
    const char* expr_str_;
    SourceLocation src_loc_;
    const char* message_;  // optional, may be nullptr

    void log_violation() noexcept {
      spdlog::critical("assertion violated: {}", expr_str_);
      spdlog::critical("  source: {}", src_loc_);
      if (message_ != nullptr) {
        spdlog::critical("  message: {}", message_);
      }
      spdlog::shutdown();
    }
  };
}

#define WDP_ASSERT(expr__, ...) \
  ::wdp::Assertion{#expr__, WDP_CURRENT_SOURCE_LOCATION, __VA_ARGS__}([&]() noexcept { return expr__; })
