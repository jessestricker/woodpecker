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

#include <memory>
#include <string>

#include <woodpecker/id.hpp>
#include <woodpecker/mesh.hpp>
#include <woodpecker/pga.hpp>

namespace wdp {
  class Part {
  public:
    explicit Part(Id id);

    auto id() const noexcept { return id_; }

    const auto& name() const noexcept { return name_; }
    void set_name(const std::string& name) { name_ = name; }

    const auto& mesh() const noexcept { return mesh_; }
    void set_mesh(const Mesh& mesh) { mesh_ = mesh; }

    const auto& motor() const noexcept { return motor_; }
    void set_motor(const kln::motor& motor) { motor_ = motor; }

  private:
    Id id_;             ///< unique id
    std::string name_;  ///< name, must not be unique
    Mesh mesh_;         ///< the mesh of the part, in local space
    kln::motor motor_{identity_motor};
  };
}
