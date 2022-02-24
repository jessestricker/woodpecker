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
#include <spdlog/spdlog.h>
#include <woodpecker/scene.hpp>

namespace wdp {
  class SceneEditor : public QObject {
    Q_OBJECT

  public:
    static constexpr float snap_angle_tolerance = (2 * pi) / 360;  // radian
    static constexpr float snap_dist_tolerance = 0.05f;            // meter

    enum class DragMode {
      along_ground_plane,
      along_vertical_axis,
    };

    void save_to_file(const std::filesystem::path& path) const;
    void load_from_file(const std::filesystem::path& path);

    const auto& scene() const { return scene_; }

    void add_part() {
      auto& part = scene_.add_part();
      part.set_mesh(Mesh::create_cuboid(1, 1, 1));
      part_added(&part);
      set_selected_part(&part);
    }

    /// may be nullptr
    const Part* selected_part() const { return selected_part_; }

    void set_selected_part(const Part* part) {
      if (selected_part_ != part) {
        selected_part_ = const_cast<Part*>(part);
        selected_part_changed(part);
      }
    }

    void start_dragging(kln::point start_point, DragMode mode) {
      WDP_ASSERT(selected_part_ != nullptr);
      is_dragging_ = true;
      drag_mode_ = mode;
      last_drag_point_ = project_drag_point(start_point);
      before_drag_ = selected_part_->motor();
      drag_motion_ = identity_motor;
    }

    void drag_selected(kln::point target) {
      WDP_ASSERT(selected_part_ != nullptr);
      if (!is_dragging_) {
        return;
      }

      // compose part motion
      const auto projected_target = project_drag_point(target);
      const auto translation = kln::sqrt(projected_target * last_drag_point_);
      drag_motion_ = translation * drag_motion_;
      last_drag_point_ = projected_target;

      // check for snapping
      drag_may_join_ = check_drag_may_join();

      // translate part
      selected_part_->set_motor(before_drag_ * drag_motion_);
      part_transformed(selected_part_);
    }

    void commit_dragging() {
      if (!is_dragging_) {
        return;
      }

      is_dragging_ = false;
      // TODO: add joint
    }

    void cancel_dragging() {
      if (!is_dragging_) {
        return;
      }

      is_dragging_ = false;
      selected_part_->set_motor(before_drag_);
    }

  signals:
    void selected_part_changed(const Part* part);
    void part_transformed(const Part* part);
    void part_added(const Part* part);
    void part_removed(const Part* part);

  private:
    Scene scene_;
    Part* selected_part_{nullptr};

    // dragging
    bool is_dragging_{false};
    DragMode drag_mode_{};
    kln::point last_drag_point_{};
    bool drag_may_join_{false};
    kln::motor before_drag_{};
    kln::motor drag_motion_{};

    kln::point project_drag_point(kln::point p) const {
      WDP_ASSERT(selected_part_ != nullptr);
      if (drag_mode_ == DragMode::along_ground_plane) {
        const auto move_plane = selected_part_->motor()(Scene::ground_plane);
        return kln::project(p, move_plane);
      } else {
        const auto move_axis = selected_part_->motor()(Scene::vertical_axis);
        return kln::project(p, move_axis);
      }
    }

    bool check_drag_may_join() {
      const auto part_motor = before_drag_ * drag_motion_;
      for (const auto& face : selected_part_->mesh().faces()) {
        const auto plane = fix_kln::normalized(part_motor(face.plane));

        for (const auto& other_part : scene().parts()) {
          if (&other_part == selected_part_) {
            continue;
          }
          for (const auto& other_face : other_part.mesh().faces()) {
            const auto other_plane = fix_kln::normalized(other_part.motor()(other_face.plane));

            // angle between planes
            const auto angle = std::acos(plane | other_plane);
            if (std::abs(angle - pi) > snap_angle_tolerance) {
              // planes are not opposite
              continue;
            }

            // distance
            const auto dist = fix_kln::ideal_norm(plane ^ other_plane);
            if (dist > snap_dist_tolerance) {
              continue;
            }

            const auto snap_motor = kln::sqrt((other_plane * plane).constrained());
            drag_motion_ = drag_motion_ * snap_motor;
            return true;
          }
        }
      }
      return false;
    }
  };
}
