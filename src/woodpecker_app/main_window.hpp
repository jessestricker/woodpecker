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

#include <QMainWindow>
#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <woodpecker/scene.hpp>

namespace wdp::app {
  class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow();

  private:
    static constexpr auto minimum_size = QSize{640, 360};
    static constexpr auto default_size = minimum_size * 2;

    Qt3DExtras::Qt3DWindow* view_;
    Qt3DCore::QEntity* view_root_;
    Qt3DCore::QEntity* scene_root_;
    Qt3DRender::QMaterial* part_material_;
    Scene scene_;

    void setup_menu_bar();
    void setup_status_bar();
    void setup_side_bar();
    void setup_ground_plane();

    // slots
    void update_view();
  };
}
