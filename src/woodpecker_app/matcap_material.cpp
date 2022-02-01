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

#include "matcap_material.hpp"

#include <string_view>

#include <QUrl>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QFilterKey>
#include <Qt3DRender/QGraphicsApiFilter>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QShaderProgram>
#include <Qt3DRender/QTechnique>

using namespace Qt3DRender;

namespace wdp::app {
  MatCapMaterial::MatCapMaterial() {
    auto* shader_program = new QShaderProgram{};
    shader_program->setVertexShaderCode(QShaderProgram::loadSource(QUrl{"qrc:/shader/matcap.vert.glsl"}));
    shader_program->setFragmentShaderCode(QShaderProgram::loadSource(QUrl{"qrc:/shader/matcap.frag.glsl"}));

    auto* render_pass = new QRenderPass{};
    render_pass->setShaderProgram(shader_program);

    auto* filter_key = new QFilterKey{};
    filter_key->setName("renderingStyle");
    filter_key->setValue("forward");

    auto* technique = new QTechnique{};
    technique->addFilterKey(filter_key);
    technique->graphicsApiFilter()->setApi(QGraphicsApiFilter::OpenGL);
    technique->graphicsApiFilter()->setProfile(QGraphicsApiFilter::CoreProfile);
    technique->graphicsApiFilter()->setMajorVersion(3);
    technique->graphicsApiFilter()->setMinorVersion(2);
    technique->addRenderPass(render_pass);

    auto* effect = new QEffect{};
    effect->addTechnique(technique);

    this->setEffect(effect);
  }
}
