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

#include <catch2/catch_test_macros.hpp>
#include <woodpecker/mesh.hpp>

using namespace wdp;

TEST_CASE("triangulate triangle mesh") {
  auto mesh = Mesh{};
  const auto face = mesh.add_face({{kln::point{0, 0, 0}}, {kln::point{1, 0, 0}}, {kln::point{0, 0, 1}}});
  const auto triangulation = mesh.triangulate();
  REQUIRE(triangulation.size() == 1);
  const auto tri_face = triangulation[0];
  REQUIRE(tri_face.plane == face.plane);
  REQUIRE(tri_face.vertices == std::array<VertexIndex, 3>{0, 1, 2});
}

TEST_CASE("triangulate quad mesh") {
  const auto mesh = Mesh::create_plane(1, 1);
  for (const auto& face : mesh.faces()) {
    const auto face_tris = mesh.triangulate_face(face);
    REQUIRE(face_tris.size() == 2);
  }
}

TEST_CASE("triangulate cube mesh") {
  const auto mesh = Mesh::create_cuboid(1, 1, 1);
  for (const auto& face : mesh.faces()) {
    const auto face_tris = mesh.triangulate_face(face);
    REQUIRE(face_tris.size() == 2);
  }
}
