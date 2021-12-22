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

#include <cstddef>
#include <initializer_list>
#include <ranges>
#include <vector>

#include <klein/klein.hpp>

namespace wdp {
  struct Vertex {
    kln::point pos{};
  };

  struct Face {
    std::vector<std::size_t> vertices;
    kln::plane plane{};
  };

  class Mesh {
  public:
    static Mesh create_cuboid(float size_x, float size_y, float size_z);

    std::size_t add_vertex(const Vertex& vertex);
    const Face& add_face(std::initializer_list<Vertex> vertices);

    const auto& vertices() const noexcept { return vertices_; }
    const auto& faces() const noexcept { return faces_; }

    auto face_vertices(const Face& face) const {
      return face.vertices | std::views::transform([&](auto&& index) { return vertices()[index]; });
    }

  private:
    static constexpr float epsilon = 1e-5F;
    std::vector<Vertex> vertices_;
    std::vector<Face> faces_;
  };
}
