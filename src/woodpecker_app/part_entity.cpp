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

#include "part_entity.hpp"

#include <spdlog/spdlog.h>

#include "util/qt.hpp"

namespace wdp::app {
  PartEntity::PartEntity(const Part* part, Qt3DCore::QNode* parent) : Qt3DCore::QEntity{parent}, part_{part} {
    // mesh
    renderer_ = new MeshRenderer{};
    update_mesh();
    addComponent(renderer_);

    // material
    material_ = new PartMaterial{};
    addComponent(material_);

    // transform
    transform_ = new Qt3DCore::QTransform{};
    update_transform();
    addComponent(transform_);
  }

  void PartEntity::set_selected(bool selected) {
    spdlog::debug("selected part({}): {}", part_->name(), selected);
    if (selected) {
      material_->set_face_color(PartMaterial::default_line_color);
    } else {
      material_->set_face_color(PartMaterial::default_face_color);
    }
  }

  void PartEntity::update_transform() {
    const auto mat = qmatrix_from_kln_motor(part_->motor());
    transform_->setMatrix(mat);
  }

  void PartEntity::update_mesh() { renderer_->update_mesh(part_->mesh()); }
}
