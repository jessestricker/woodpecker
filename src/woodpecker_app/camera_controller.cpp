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

#include "camera_controller.hpp"

#include <Qt3DRender/QCamera>

using namespace Qt3DExtras;
using namespace Qt3DRender;

namespace wdp::app {
  CameraController::CameraController(Qt3DCore::QNode* parent) : QAbstractCameraController{parent} {}

  void CameraController::moveCamera(const QAbstractCameraController::InputState& state, float dt) {
    auto* cam = camera();
    if (cam == nullptr) {
      return;
    }

    // state:
    // rx, ry - mouse movement
    // tx, ty - arrow keys
    //     tz - scroll wheel

    // controls:
    //         MMB + move mouse - rotate eye around ref
    // shift + MMB + move mouse - translate eye and ref
    //             scroll wheel - zoom (actually dolly)

    constexpr auto up = QVector3D{0, 1, 0};
    constexpr auto min_view_dist = 1.0f;

    if (state.middleMouseButtonActive) {
      if (state.shiftKeyActive) {
        // translate eye and ref
        const auto move = QVector3D{state.rxAxisValue, state.ryAxisValue, 0} * linearSpeed() * dt * -1;
        cam->translate(move, QCamera::TranslateViewCenter);
      } else {
        // rotate eye around ref
        cam->panAboutViewCenter((state.rxAxisValue * lookSpeed()) * dt * -1, up);
        cam->tiltAboutViewCenter((state.ryAxisValue * lookSpeed()) * dt * -1);
      }
    } else {
      // zoom (actually dolly)
      const auto move = QVector3D{0, 0, state.tzAxisValue} * zoom_speed() * dt;
      cam->translate(move, QCamera::DontTranslateViewCenter);
      const auto view_dist = cam->viewVector().length();
      const auto view_dist_delta = view_dist - min_view_dist;
      if (view_dist_delta < 0) {
        // dolly back to prevent view inversion
        cam->translate({0, 0, view_dist_delta}, QCamera::DontTranslateViewCenter);
      }
    }
  }
}
