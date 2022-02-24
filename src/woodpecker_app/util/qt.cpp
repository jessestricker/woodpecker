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

#include "qt.hpp"

#include <algorithm>

namespace wdp::app {
  QString qstring_from_sv(std::string_view sv) {
    const auto sv_size = narrow<qsizetype>(sv.size());
    return QString::fromUtf8(sv.data(), sv_size);
  }

  QMatrix4x4 qmatrix_from_kln_motor(const kln::motor& m) {
    const auto kln_mat = m.as_mat4x4();
    auto qmat = QMatrix4x4{};
    std::copy_n(std::begin(kln_mat.data), 4 * 4, qmat.data());
    return qmat;
  }

  kln::point point_from_qvec(const QVector3D& vec) { return kln::point{vec.x(), vec.y(), vec.z()}; }
}
