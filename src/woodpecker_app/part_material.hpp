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

#include <string_view>

#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QParameter>

namespace wdp::app {
  class PartMaterial : public Qt3DRender::QMaterial {
    Q_OBJECT

  public:
    static constexpr QRgb default_face_color = QRgb{0xd5c4b5};   // HSLuv: 50,20,80
    static constexpr QRgb selected_face_color = QRgb{0xa88c6f};  // HSLuv: 50,40,60
    static constexpr QRgb default_line_color = QRgb{0x1d1b19};   // HSLuv: 50,10,10

    explicit PartMaterial(Qt3DCore::QNode* parent = nullptr);

    void set_face_color(QColor face_color);
    void set_line_color(QColor line_color);

  private:
    static constexpr std::string_view face_color_name_ = "faceColor";
    static constexpr std::string_view line_color_name_ = "lineColor";

    Qt3DRender::QParameter* face_color_param_;
    Qt3DRender::QParameter* line_color_param_;
  };
}
