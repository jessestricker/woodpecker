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

template <class T>
void append_as_bytes(QByteArray& arr, std::array<T, 3> data) {
  const auto* bytes_ptr = reinterpret_cast<const char*>(data.data());
  const auto bytes_size = data.size() * sizeof(T);
  arr.append(bytes_ptr, bytes_size);
}

namespace wdp::app {
  MeshRenderer::MeshRenderer(QNode* parent) : QGeometryRenderer{parent} {}

  void MeshRenderer::update_mesh(const Mesh& mesh) {
    const auto triangles = mesh.triangulate();

    // create vertex data array
    auto vtx_data = QByteArray{};
    auto idx_data = QByteArray{};
    uint index = 0;
    for (const auto& triangle : triangles) {
      const auto triangle_nrm = -fix_kln::polarity(fix_kln::normalized(triangle.plane));
      for (const auto& vtx_idx : triangle.vertices) {
        auto vtx_pos = fix_kln::normalized(mesh.vertices()[vtx_idx].pos);
        append_as_bytes<float>(vtx_data, {vtx_pos.x(), vtx_pos.y(), vtx_pos.z()});
        append_as_bytes<float>(vtx_data, {triangle_nrm.x(), triangle_nrm.y(), triangle_nrm.z()});
      }
      append_as_bytes<uint>(idx_data, {index, index + 1, index + 2});
      index += 3;
    }

    // load vertex data array into buffer
    auto* vertex_buffer = new QBuffer{this};
    vertex_buffer->setData(vtx_data);
    auto* index_buffer = new QBuffer{this};
    index_buffer->setData(idx_data);

    const auto stride = narrow<uint>((3 + 3) * sizeof(float));  // pos (vec3), nrm (vec3)

    // create vertex position attribute
    auto* vtx_pos_attr = new QAttribute{this};
    vtx_pos_attr->setAttributeType(QAttribute::VertexAttribute);
    vtx_pos_attr->setName(QAttribute::defaultPositionAttributeName());
    vtx_pos_attr->setVertexBaseType(QAttribute::Float);
    vtx_pos_attr->setVertexSize(3);
    vtx_pos_attr->setCount(narrow<uint>(triangles.size() * 3));
    vtx_pos_attr->setBuffer(vertex_buffer);
    vtx_pos_attr->setByteStride(stride);

    // create vertex normal attribute
    auto* vtx_nrm_attr = new QAttribute{this};
    vtx_nrm_attr->setAttributeType(QAttribute::VertexAttribute);
    vtx_nrm_attr->setName(QAttribute::defaultNormalAttributeName());
    vtx_nrm_attr->setVertexBaseType(QAttribute::Float);
    vtx_nrm_attr->setVertexSize(3);
    vtx_nrm_attr->setCount(narrow<uint>(triangles.size() * 3));
    vtx_nrm_attr->setBuffer(vertex_buffer);
    vtx_nrm_attr->setByteStride(stride);
    vtx_nrm_attr->setByteOffset(3 * sizeof(float));

    // create index attribute
    auto* idx_attr = new QAttribute{this};
    idx_attr->setAttributeType(QAttribute::IndexAttribute);
    idx_attr->setVertexBaseType(QAttribute::UnsignedInt);
    idx_attr->setBuffer(index_buffer);
    idx_attr->setCount(narrow<uint>(triangles.size() * 3));

    // geometry
    auto* geometry = new QGeometry{};
    geometry->addAttribute(vtx_pos_attr);
    geometry->addAttribute(vtx_nrm_attr);
    // geometry->addAttribute(idx_attr);
    this->setGeometry(geometry);
  }
}
