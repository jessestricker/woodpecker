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
#include <woodpecker/part.hpp>

#include "mesh_renderer.hpp"
#include "part_material.hpp"

namespace wdp::app {
  class PartEntity : public Qt3DCore::QEntity {
    Q_OBJECT

  public:
    PartEntity(const Part* part, Qt3DCore::QNode* parent);

    const auto* part() const { return part_; };

    void set_selected(bool selected);
    void update_transform();
    void update_mesh();

  private:
    const Part* part_;
    PartMaterial* material_;
    MeshRenderer* renderer_;
    Qt3DCore::QTransform* transform_;
  };
}
