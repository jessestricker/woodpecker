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

#include <boost/circular_buffer.hpp>
#include <woodpecker/util/assert.hpp>
#include <woodpecker/util/cast.hpp>

namespace wdp {
  Mesh Mesh::create_plane(float size_x, float size_z) {
    const auto t_x = kln::translator{size_x, 1, 0, 0};
    const auto t_z = kln::translator{size_z, 0, 0, 1};

    const auto p_0 = kln::point{size_x / -2, 0, size_z / -2};
    const auto p_x = t_x(p_0);
    const auto p_xz = t_z(p_x);
    const auto p_z = t_z(p_0);

    auto mesh = Mesh{};
    mesh.add_face({{p_0}, {p_z}, {p_xz}, {p_x}});
    return mesh;
  }

  Mesh Mesh::create_cuboid(float size_x, float size_y, float size_z) {
    const auto t_x = kln::translator{size_x, 1, 0, 0};
    const auto t_y = kln::translator{size_y, 0, 1, 0};
    const auto t_z = kln::translator{size_z, 0, 0, 1};

    // points
    const auto p_0 = kln::point{size_x / -2, size_y / -2, size_z / -2};
    const auto p_x = t_x(p_0);
    const auto p_y = t_y(p_0);
    const auto p_z = t_z(p_0);
    const auto p_xy = t_y(p_x);
    const auto p_yz = t_z(p_y);
    const auto p_zx = t_x(p_z);
    const auto p_xyz = t_z(p_xy);

    // faces
    auto mesh = Mesh{};
    mesh.add_face({{p_0}, {p_z}, {p_yz}, {p_y}});     // left
    mesh.add_face({{p_x}, {p_xy}, {p_xyz}, {p_zx}});  // right
    mesh.add_face({{p_0}, {p_x}, {p_zx}, {p_z}});     // bottom
    mesh.add_face({{p_y}, {p_yz}, {p_xyz}, {p_xy}});  // top
    mesh.add_face({{p_0}, {p_y}, {p_xy}, {p_x}});     // front
    mesh.add_face({{p_z}, {p_zx}, {p_xyz}, {p_yz}});  // back
    return mesh;
  }

  VertexIndex Mesh::add_vertex(const Vertex& new_vtx) {
    // check if mesh has existing vertex at same position
    const auto new_vtx_pos = new_vtx.pos.normalized();
    const auto is_at_same_pos = [&](const Vertex& old_vtx) {
      return (old_vtx.pos.normalized() & new_vtx_pos).norm() <= merge_dist;
    };
    const auto iter = std::ranges::find_if(vertices_, is_at_same_pos);
    if (iter != vertices_.end()) {
      // return index of existing vertex
      return narrow<VertexIndex>(std::ranges::distance(vertices_.begin(), iter));
    }
    // add new vertex to list
    vertices_.push_back(new_vtx);
    return narrow<VertexIndex>(vertices_.size() - 1);
  }

  const Face& Mesh::add_face(const std::vector<Vertex>& vertices) {
    WDP_ASSERT(vertices.size() >= 3);

    // construct normalized plane of first 3 vertices
    const auto p = (vertices[0].pos & vertices[1].pos & vertices[2].pos);
    const auto plane = fix_kln::normalized(p);

    // add vertices
    auto vertex_indices = std::vector<VertexIndex>{};
    for (const auto& vtx : vertices) {
      const auto join = (fix_kln::normalized(vtx.pos) & plane);
      const auto vtx_plane_dist = std::abs(join.scalar());
      WDP_ASSERT(vtx_plane_dist < merge_dist, "vertex must be in plane");

      const auto vertex_index = add_vertex(vtx);
      vertex_indices.push_back(vertex_index);
    }

    // add face
    const auto new_face = Face{vertex_indices, plane};
    faces_.push_back(new_face);
    return faces_.back();
  }

  std::vector<TriFace> Mesh::triangulate() const {
    auto all_tri_faces = std::vector<TriFace>{};
    for (const auto& face : faces_) {
      const auto tri_faces = triangulate_face(face);
      std::ranges::copy(tri_faces, std::back_inserter(all_tri_faces));
    }
    return all_tri_faces;
  }

  std::vector<TriFace> Mesh::triangulate_face(const Face& face) const {
    auto ears = std::vector<TriFace>{};
    auto polygon = boost::circular_buffer<VertexIndex>{face.vertices.begin(), face.vertices.end()};

    // clip ears until polygon is a single triangle
    while (polygon.size() > 3) {
      for (auto iter = polygon.begin(); iter != polygon.end(); ++iter) {
        // look at three consecutive vertices in polygon, check if it is an ear
        const auto idx = *iter;
        const auto vtx = vertices_[idx].pos;
        const auto idx_prev = *(iter - 1);
        const auto vtx_prev = vertices_[idx_prev].pos;
        const auto idx_next = *(iter + 1);
        const auto vtx_next = vertices_[idx_next].pos;

        // compute angle at vertex v_i
        const auto edge_vp_to_v = vtx_prev & vtx;
        const auto edge_v_to_vn = vtx & vtx_next;
        const auto angle = std::acos(fix_kln::normalized(edge_vp_to_v) | fix_kln::normalized(edge_v_to_vn));

        // cond 1: check if convex
        const auto is_convex = angle < kln::pi;
        if (!is_convex) {
          continue;
        }

        // cond 2: check if other vertices lie in triangle
        const auto plane_vp_to_v = face.plane | edge_vp_to_v;  // plane orthogonal to face on edge
        const auto plane_v_to_vn = face.plane | edge_v_to_vn;
        const auto plane_vn_to_vp = face.plane | (vtx_next & vtx_prev);
        bool any_other_inside_triangle = false;
        for (auto other_iter = polygon.begin(); other_iter != polygon.end(); ++other_iter) {
          if (other_iter == iter || other_iter == (iter - 1) || other_iter == (iter + 1)) {
            continue;
          }
          const auto other_vtx = vertices_[*other_iter].pos;

          // check on which side of the planes the vertex lies
          const auto meet_vp_to_v = (other_vtx ^ plane_vp_to_v).scalar();
          const auto meet_v_to_vn = (other_vtx ^ plane_v_to_vn).scalar();
          const auto meet_vn_to_vp = (other_vtx ^ plane_vn_to_vp).scalar();

          // if all meets are positive, the point v_j is inside
          if (meet_vp_to_v > 0 && meet_v_to_vn > 0 && meet_vn_to_vp > 0) {
            any_other_inside_triangle = true;
            break;
          }
        }
        if (any_other_inside_triangle) {
          continue;
        }

        // clip ear, remove idx from polygon
        ears.push_back({{idx_prev, idx, idx_next}, face.plane});
        polygon.erase(iter);
        break;
      }
    }

    // add remaining triangle to clipped ears
    ears.push_back({{polygon[0], polygon[1], polygon[2]}, face.plane});

    return ears;
  }
}
