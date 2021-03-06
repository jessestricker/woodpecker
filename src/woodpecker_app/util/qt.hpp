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

#include <span>
#include <string_view>
#include <vector>

#include <QByteArray>
#include <QMatrix4x4>
#include <QString>
#include <klein/motor.hpp>
#include <woodpecker/util/cast.hpp>

namespace wdp::app {
  QString qstring_from_sv(std::string_view sv);

  template <class Element>
  QByteArray qbyte_array_from_vector(const std::vector<Element>& vec) {
    const auto span = std::span{vec};
    const auto bytes_span = std::as_bytes(span);
    return QByteArray{reinterpret_cast<const char*>(bytes_span.data()), narrow<qsizetype>(bytes_span.size())};
  }

  QMatrix4x4 qmatrix_from_kln_motor(const kln::motor& m);
}
