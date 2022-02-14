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

#include "mesh_renderer.hpp"

#include <array>

#include "util/qt.hpp"

using namespace Qt3DCore;
using namespace Qt3DRender;

namespace wdp::app {
  MeshRenderer::MeshRenderer(QNode* parent) : QGeometryRenderer{parent} {}

  void MeshRenderer::update_mesh(const Mesh& mesh) {
    // vertices
    auto* vertex_buffer = new QBuffer{};
    auto vertex_data = QByteArray{};
    for (const auto& vtx : mesh.vertices()) {
      // normalize
      const auto np = fix_kln::normalized(vtx.pos);
      // swizzle
      auto xyzw = std::array<float, 4>{np.x(), np.y(), np.z(), np.w()};
      // store to buffer
      vertex_data.append(reinterpret_cast<const char*>(xyzw.data()), xyzw.size() * sizeof(float));
    }
    vertex_buffer->setData(vertex_data);

    auto* vertex_attr = new QAttribute{vertex_buffer, QAttribute::defaultPositionAttributeName(), QAttribute::Float, 4,
                                       narrow<uint>(mesh.vertices().size())};
    vertex_attr->setAttributeType(QAttribute::VertexAttribute);

    // indices
    const auto triangle_indices = mesh.triangulate();
    auto* index_buffer = new QBuffer{};
    index_buffer->setData(qbyte_array_from_vector(triangle_indices));
    auto* index_attr = new QAttribute{index_buffer, QAttribute::defaultPositionAttributeName(), QAttribute::UnsignedInt,
                                      1, narrow<uint>(triangle_indices.size() * 3)};
    index_attr->setAttributeType(QAttribute::IndexAttribute);

    // geometry
    auto* geometry = new QGeometry{};
    geometry->addAttribute(vertex_attr);
    geometry->addAttribute(index_attr);

    this->setGeometry(geometry);
  }
}
