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

#include "main_window.hpp"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QGoochMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QGeometryRenderer>
#include <woodpecker/config.hpp>

#include "matcap_material.hpp"
#include "util/qt.hpp"

using namespace Qt3DRender;
using namespace Qt3DExtras;
using namespace Qt3DCore;

namespace {
  using namespace wdp;

  Scene load_example() {
    auto scene = Scene{};
    auto mesh = Mesh::create_cuboid(1, 1, 1);
    auto part = Part{mesh};
    part.set_motor(kln::motor{kln::translator{2, 0, 1, 0}});
    scene.add_part(part);
    return scene;
  }

  QGeometryRenderer* wdp_mesh_to_qt_geo(const Mesh& mesh) {
    // vertices
    auto* vertex_buffer = new QBuffer{};
    vertex_buffer->setData(app::qbyte_array_from_vector(mesh.vertices()));
    auto* vertex_attr = new QAttribute{vertex_buffer, QAttribute::defaultPositionAttributeName(), QAttribute::Float, 4,
                                       narrow<uint>(mesh.vertices().size())};
    vertex_attr->setAttributeType(QAttribute::VertexAttribute);

    // indices
    const auto triangle_indices = mesh.triangulate();
    auto* index_buffer = new QBuffer{};
    index_buffer->setData(app::qbyte_array_from_vector(triangle_indices));
    auto* index_attr = new QAttribute{index_buffer, QAttribute::defaultPositionAttributeName(), QAttribute::UnsignedInt,
                                      1, narrow<uint>(triangle_indices.size() * 3)};
    index_attr->setAttributeType(QAttribute::IndexAttribute);

    // geometry and renderer
    auto* geometry = new QGeometry{};
    geometry->addAttribute(vertex_attr);
    geometry->addAttribute(index_attr);
    auto* geometry_renderer = new QGeometryRenderer{};
    geometry_renderer->setGeometry(geometry);

    return geometry_renderer;
  }
}

namespace wdp::app {
  MainWindow::MainWindow() {
    // setup window
    setWindowTitle(qstring_from_sv(project_name));
    setMinimumSize(minimum_size);
    resize(default_size);
    setup_menu_bar();
    setup_status_bar();
    setup_side_bar();

    // setup 3D view
    view_ = new Qt3DWindow{};
    setCentralWidget(QWidget::createWindowContainer(view_, this));
    view_->defaultFrameGraph()->setClearColor(0x6e95b8);
    view_->defaultFrameGraph()->setShowDebugOverlay(true);

    // setup root entities
    view_root_ = new QEntity{};
    view_->setRootEntity(view_root_);
    scene_root_ = new QEntity{view_root_};
    setup_ground_plane();
    part_material_ = new MatCapMaterial{};

    // setup camera
    view_->camera()->setPosition({-4, 2, -4});
    view_->camera()->setViewCenter({0, 0, 0});
    auto* camera_ctrl = new QOrbitCameraController{view_root_};
    camera_ctrl->setCamera(view_->camera());

    // load example scene
    scene_ = load_example();
    update_view();
  }

  void MainWindow::setup_menu_bar() {
    auto* file = menuBar()->addMenu("File");
    file->addAction("New...");
    file->addAction("Open...");
    file->addSeparator();
    file->addAction("Save");
    file->addAction("Save as...");
    file->addSeparator();
    auto* exit_act = file->addAction("Exit");
    connect(exit_act, &QAction::triggered, QApplication::instance(), &QApplication::quit, Qt::QueuedConnection);
  }

  void MainWindow::setup_status_bar() { statusBar()->addWidget(new QLabel{"Ready"}); }

  void MainWindow::setup_side_bar() {
    auto* outline = new QDockWidget{"Outline", this};
    outline->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    outline->setFeatures(QDockWidget::DockWidgetMovable);
    outline->setMinimumWidth(120);
    addDockWidget(Qt::RightDockWidgetArea, outline);
  }

  void MainWindow::setup_ground_plane() {
    auto* mesh = new QPlaneMesh{};
    mesh->setWidth(10);
    mesh->setHeight(10);

    auto* material = new QGoochMaterial{};
    material->setDiffuse(0xaaaaaa);

    auto* ground_plane = new QEntity{view_root_};
    ground_plane->addComponent(mesh);
    ground_plane->addComponent(material);
  }

  void MainWindow::update_view() {
    // clear scene
    delete scene_root_;
    scene_root_ = new QEntity{view_root_};

    // add entity for each scene part
    for (const auto& part : scene_.parts()) {
      // mesh
      auto* geo_render = wdp_mesh_to_qt_geo(part.mesh());

      // transform
      auto* transform = new Qt3DCore::QTransform();
      const auto tfm_matrix = qmatrix_from_kln_motor(part.motor());
      transform->setMatrix(tfm_matrix);

      // build entity
      auto* entity = new QEntity{scene_root_};
      entity->addComponent(geo_render);
      entity->addComponent(part_material_);
      entity->addComponent(transform);
    }
  }
}
