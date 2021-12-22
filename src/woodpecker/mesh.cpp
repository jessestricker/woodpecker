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

#include "mesh.hpp"

#include <algorithm>

#include <woodpecker/util/assert.hpp>

namespace wdp {
  Mesh Mesh::create_cuboid(float size_x, float size_y, float size_z) {
    // TODO: add 6 faces
    return {};
  }

  std::size_t Mesh::add_vertex(const Vertex& vertex) {
    // find vertex at position of new vertex
    const auto at_same_pos = [&](const Vertex& elem) {
      return (elem.pos.normalized() & vertex.pos.normalized()).norm() <= epsilon;
    };
    const auto it = std::ranges::find_if(vertices_, at_same_pos);
    if (it != vertices_.end()) {
      // return index of existing vertex
      return std::ranges::distance(vertices_.begin(), it);
    }
    // add new vertex to set
    vertices_.push_back(vertex);
    return vertices_.size() - 1;
  }

  const Face& Mesh::add_face(std::initializer_list<Vertex> vertices) {
    WDP_ASSERT(vertices.size() >= 3);
    auto iter = vertices.begin();
    const auto plane = (*iter++).pos & (*iter++).pos & (*iter++).pos;
    // construct plane of first 3 vertices
    // assert all other vertices lie in plane
    // add vertices or reuse existing ones
    // add face
    return faces_[0];
  }
}
