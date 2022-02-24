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

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DInput>
#include <Qt3DRender>
#include <QtWidgets>
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

    QListWidget* outline_{nullptr};

    void setup_menu_bar();
    void setup_status_bar();
    void setup_outline();
    void setup_tool_bar();
    void setup_ground_plane();

    kln::line create_screen_ray(const QPoint& screen_pos);

    // slots:
    void scene_part_added(const Part* part);
    void part_clicked(Qt3DRender::QPickEvent* event);
    void part_mouse_pressed(Qt3DRender::QPickEvent* event);
    void part_mouse_moved(Qt3DRender::QPickEvent* event);
    void part_mouse_released(Qt3DRender::QPickEvent* event);
  };
}
