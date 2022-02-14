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

#include <filesystem>

#include <QObject>
#include <woodpecker/scene.hpp>

namespace wdp {
  class SceneEditor : public QObject {
    Q_OBJECT

  public:
    void save_to_file(const std::filesystem::path& path) const;
    void load_from_file(const std::filesystem::path& path);

    const auto& scene() const { return scene_; }

    const Part* add_part() {
      auto& part = scene_.add_part();
      part.set_mesh(Mesh::create_cuboid(1, 1, 1));
      return &part;
    }

    /// may be nullptr
    const auto* selected_part() const { return selected_part_; }

    void set_selected_part(const Part* part) {
      if (selected_part_ != part) {
        selected_part_ = part;
        selected_part_changed(part);
      }
    }

    void move_selected_along_line(kln::point dst);

    void move_selected_in_plane(kln::point dst) {
      if (!selected_part_) return;

      // construct plane parallel to ground through part
      const auto move_plane = selected_part_->motor()(Scene::ground_plane);
      // project destination onto plane
      const auto constrained_dst = kln::project(dst, move_plane);
    }

  signals:
    void selected_part_changed(const Part* part);

  private:
    Scene scene_;
    const Part* selected_part_{nullptr};
  };
}
