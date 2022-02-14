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
#include <QToolBar>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QGoochMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QTextureMaterial>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>
#include <Qt3DRender/QPickingSettings>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QTexture>
#include <spdlog/spdlog.h>
#include <woodpecker/config.hpp>
#include <woodpecker/util/assert.hpp>

#include "part_entity.hpp"
#include "util/qt.hpp"

using namespace Qt3DRender;
using namespace Qt3DExtras;
using namespace Qt3DCore;

namespace wdp::app {
  MainWindow::MainWindow() {
    // setup window
    setWindowTitle(qstring_from_sv(project_name));
    setMinimumSize(minimum_size);
    resize(default_size);
    setup_menu_bar();
    setup_status_bar();
    setup_side_bar();
    setup_tool_bar();

    // setup 3D view
    view_ = new Qt3DWindow{nullptr, API::OpenGL};
    auto* view_container = QWidget::createWindowContainer(view_, this);
    view_container->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(view_container);
    view_->defaultFrameGraph()->setClearColor(0x6e95b8);
    view_->defaultFrameGraph()->setShowDebugOverlay(false);

    // setup root entities
    view_root_ = new QEntity{};
    view_->setRootEntity(view_root_);
    setup_ground_plane();
    scene_root_ = new QEntity{view_root_};
    scene_root_->setObjectName("scene root");

    // setup camera
    view_->camera()->setPosition({-4, 2, -4});
    view_->camera()->setViewCenter({0, 0, 0});
    auto* camera_ctrl = new QOrbitCameraController{view_root_};
    camera_ctrl->setCamera(view_->camera());

    // setup picking
    view_->renderSettings()->pickingSettings()->setPickMethod(QPickingSettings::TrianglePicking);
    auto* picker = new QObjectPicker{};
    picker->setDragEnabled(true);
    picker->setHoverEnabled(false);
    QObject::connect(picker, &QObjectPicker::clicked, this, &MainWindow::click_part);
    QObject::connect(picker, &QObjectPicker::moved, this, &MainWindow::drag_part);

    scene_root_->addComponent(picker);
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

  void MainWindow::setup_tool_bar() {
    auto* toolbar = new QToolBar{"Tools"};
    toolbar->addAction("Add", this, &MainWindow::add_part_to_scene);
    toolbar->addAction("Clear", this, [this]() {
      for (auto* child : scene_root_->childNodes()) {
        delete child;
      }
    });
    addToolBar(Qt::LeftToolBarArea, toolbar);
  }

  void MainWindow::setup_ground_plane() {
    auto* ground_plane = new QEntity{view_root_};

    auto* mesh = new QPlaneMesh{ground_plane};
    mesh->setWidth(10);
    mesh->setHeight(10);
    ground_plane->addComponent(mesh);

    auto* material = new QTextureMaterial{ground_plane};
    material->setAlphaBlendingEnabled(true);
    auto* texture = new QTextureLoader{material};
    texture->setSource(QUrl{"qrc:/images/grid_cross.png"});
    texture->setMagnificationFilter(QAbstractTexture::Nearest);
    material->setTexture(texture);

    ground_plane->addComponent(material);
  }

  void MainWindow::click_part(QPickEvent* event) {
    auto* part_entity = qobject_cast<PartEntity*>(event->entity());
    if (part_entity == nullptr) {
      return;
    }

    switch (event->button()) {
      case QPickEvent::LeftButton:
        editor_.set_selected_part(part_entity->part());
        break;

      default:
        // do nothing
        break;
    }
  }

  void MainWindow::drag_part(QPickEvent* event) {
    spdlog::info("move event");
    auto* part_entity = qobject_cast<PartEntity*>(event->entity());
    if (part_entity == nullptr) {
      return;
    }
    if (event->button() != QPickEvent::LeftButton) {
      // drag only with left mouse
      return;
    }
    if (editor_.selected_part() != part_entity->part()) {
      // only move selected part
      return;
    }
    spdlog::info("dragging");
    qDebug() << "drag:" << event->worldIntersection();
  }

  void MainWindow::add_part_to_scene() {
    auto* part = editor_.add_part();
    auto* entity = new PartEntity{part, scene_root_};
    QObject::connect(&editor_, &SceneEditor::selected_part_changed, entity,
                     [entity](const Part* part) { entity->set_selected(entity->part() == part); });
  }
}
