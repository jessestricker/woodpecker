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

#include <array>
#include <cstddef>
#include <vector>

#include <woodpecker/pga.hpp>

namespace wdp {
  /// A vertex in the Mesh.
  struct Vertex {
    kln::point pos{};  ///< The position of the vertex.
  };

  /// An index to a vertex in the Mesh.
  using VertexIndex = unsigned;

  /// A polygonal face of at least 3 vertices in the Mesh.
  struct Face {
    std::vector<VertexIndex> vertices;  ///< The list of indices into the vertex list of the mesh.
    kln::plane plane{};                 ///< The plane in which all vertices of this face lie, normalized.
  };

  /// A triangular face of exactly 3 vertex indices.
  using TriFace = std::array<VertexIndex, 3>;

  /// A polygonal mesh built from the faces and vertices.
  /// The vertices are positioned in 3D space.
  /// A face is a coplanar simple polygon of at least 3 vertices.
  /// The winding order is clockwise.
  class Mesh {
  public:
    /// The maximum distance threshold below which any two vertices are merged into one.
    static constexpr auto merge_dist = 1e-5F;

    /// Creates a plane mesh at the origin in the xz-plane facing y-up.
    static Mesh create_plane(float size_x, float size_z);

    /// Creates a cuboid mesh at the origin.
    static Mesh create_cuboid(float size_x, float size_y, float size_z);

    /// Adds a new vertex to the mesh.
    /// If there is already an existing vertex in the mesh, which is not further than
    /// #merge_dist apart from the new vertex, then the new vertex is not added to mesh.
    /// \return The index of the newly added vertex, or the existing vertex at the same position.
    VertexIndex add_vertex(const Vertex& new_vtx);

    /// Adds a new face to the mesh.
    /// \param vertices 3 or more coplanar vertices spanning the face.
    const Face& add_face(const std::vector<Vertex>& vertices);

    /// Creates a triangulation of this mesh.
    /// \return A list of index triples forming triangles.
    std::vector<TriFace> triangulate() const;

    const auto& vertices() const noexcept { return vertices_; }
    const auto& faces() const noexcept { return faces_; }

  private:
    std::vector<Vertex> vertices_;
    std::vector<Face> faces_;

    std::vector<TriFace> triangulate_face(const Face& face) const;
  };
}
