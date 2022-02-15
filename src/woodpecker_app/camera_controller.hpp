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

#include <Qt3DExtras/QAbstractCameraController>

namespace wdp::app {
  class CameraController : public Qt3DExtras::QAbstractCameraController {
    Q_OBJECT

  public:
    explicit CameraController(Qt3DCore::QNode* parent = nullptr);

    auto zoom_speed() const { return zoom_speed_; }
    void set_zoom_speed(float zoom_speed) { zoom_speed_ = zoom_speed; }

  protected:
    void moveCamera(const InputState& state, float dt) override;

  private:
    float zoom_speed_ = 20;
  };
}