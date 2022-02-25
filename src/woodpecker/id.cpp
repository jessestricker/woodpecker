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

#include "id.hpp"

#include <stdexcept>

namespace wdp {
  Id IdPool::operator()() {
    const auto lock = std::lock_guard{mutex_};

    const auto new_value = ++last_value_;
    if (new_value == 0) {
      // decrease, so that next time this method is invoked it throws again
      --last_value_;

      throw std::runtime_error{"unique ids exhausted"};
    }

    return Id{new_value};
  }
}
