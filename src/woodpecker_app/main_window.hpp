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
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QPickEvent>
#include <woodpecker/scene_editor.hpp>

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
    SceneEditor editor_;

    void setup_menu_bar();
    void setup_status_bar();
    void setup_side_bar();
    void setup_tool_bar();
    void setup_ground_plane();

    void click_part(Qt3DRender::QPickEvent* event);
    void drag_part(Qt3DRender::QPickEvent* event);
    void add_part_to_scene();
  };
}
