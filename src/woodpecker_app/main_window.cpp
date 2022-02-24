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

#include <woodpecker/config.hpp>

#include "camera_controller.hpp"
#include "part_entity.hpp"
#include "util/flags.hpp"
#include "util/qt.hpp"

using namespace Qt3DRender;
using namespace Qt3DExtras;
using namespace Qt3DCore;

namespace wdp::app {
  MainWindow::MainWindow() {
    // set up window
    setWindowTitle(qstring_from_sv(project_name));
    setMinimumSize(minimum_size);
    resize(default_size);
    setup_menu_bar();
    setup_status_bar();
    setup_outline();
    setup_tool_bar();

    // set up 3D view
    view_ = new Qt3DWindow{};
    view_->defaultFrameGraph()->setClearColor(QColor{0x6e95b8});
    view_->defaultFrameGraph()->setShowDebugOverlay(true);
    auto* view_container = QWidget::createWindowContainer(view_);
    setCentralWidget(view_container);

    // set up root entities
    view_root_ = new QEntity{};
    view_->setRootEntity(view_root_);
    setup_ground_plane();
    scene_root_ = new QEntity{view_root_};

    // setup input devices
    // auto* mouse_device = new Qt3DInput::QMouseDevice{scene_root_};
    // auto* mouse_handler = new Qt3DInput::QMouseHandler{};
    // mouse_handler->setSourceDevice(mouse_device);
    // QObject::connect(mouse_handler, &Qt3DInput::QMouseHandler::clicked, [this](Qt3DInput::QMouseEvent* event) {
    //  auto ray = create_screen_ray(QPoint{event->x(), event->y()});
    //  spdlog::debug("clicked");
    //});
    // scene_root_->addComponent(mouse_handler);

    // set up camera
    view_->camera()->setPosition({-6, 3, -6});
    view_->camera()->setViewCenter({0, 0, 0});
    auto* camera_ctrl = new CameraController{view_root_};
    camera_ctrl->setCamera(view_->camera());

    // set up sunlight, follows the camera
    auto* sun = new QEntity{view_root_};
    auto* sun_light = new QDirectionalLight{sun};
    sun_light->setIntensity(2);
    sun_light->setColor(QColorConstants::White);
    sun_light->setWorldDirection(view_->camera()->viewVector());
    sun->addComponent(sun_light);
    auto* sun_tfm = new Qt3DCore::QTransform{sun};
    sun_tfm->setTranslation(view_->camera()->position());
    sun->addComponent(sun_tfm);
    QObject::connect(view_->camera(), &QCamera::positionChanged, sun_tfm, &Qt3DCore::QTransform::setTranslation);
    QObject::connect(view_->camera(), &QCamera::viewVectorChanged, sun_light, &QDirectionalLight::setWorldDirection);

    // set up object picking
    view_->renderSettings()->pickingSettings()->setPickMethod(QPickingSettings::TrianglePicking);
    auto* picker = new QObjectPicker{};
    picker->setDragEnabled(true);
    picker->setHoverEnabled(false);
    QObject::connect(picker, &QObjectPicker::clicked, this, &MainWindow::part_clicked);
    QObject::connect(picker, &QObjectPicker::pressed, this, &MainWindow::part_mouse_pressed);
    QObject::connect(picker, &QObjectPicker::moved, this, &MainWindow::part_mouse_moved);
    QObject::connect(picker, &QObjectPicker::released, this, &MainWindow::part_mouse_released);
    scene_root_->addComponent(picker);

    // connect editor
    QObject::connect(&editor_, &SceneEditor::part_added, this, &MainWindow::scene_part_added);
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

  void MainWindow::setup_outline() {
    outline_ = new QListWidget{};
    outline_->setSelectionMode(QAbstractItemView::NoSelection);

    auto* container = new QDockWidget{"Outline", this};
    container->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    container->setFeatures(QDockWidget::DockWidgetMovable);
    container->setMinimumWidth(120);
    container->setWidget(outline_);
    addDockWidget(Qt::RightDockWidgetArea, container);
  }

  void MainWindow::setup_tool_bar() {
    auto* toolbar = new QToolBar{"Tools"};
    toolbar->addAction("Add", &editor_, &SceneEditor::add_part);
    toolbar->addSeparator();

    auto* translate_action = toolbar->addAction("Translate");
    translate_action->setCheckable(true);
    translate_action->setChecked(true);
    auto* rotate_action = toolbar->addAction("Rotate");
    rotate_action->setCheckable(true);
    auto* transform_group = new QActionGroup{toolbar};
    transform_group->addAction(translate_action);
    transform_group->addAction(rotate_action);

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
    texture->setSource(QUrl{"qrc:/images/grid.png"});
    texture->setMagnificationFilter(QAbstractTexture::Nearest);
    material->setTexture(texture);

    ground_plane->addComponent(material);
  }

  kln::line MainWindow::create_screen_ray(const QPoint& screen_pos) {
    const auto viewport = view_->defaultFrameGraph()->viewportRect();
    // FIXME: viewport coords are [0,0] - [1,1], we need actual size
    const auto screen_pos_x = static_cast<float>(screen_pos.x());
    const auto screen_pos_y = static_cast<float>(viewport.height() - screen_pos.y());

    // in screen space
    auto pos_near = QVector3D{screen_pos_x, screen_pos_y, 0};
    auto pos_far = QVector3D{screen_pos_x, screen_pos_y, 1};

    // in world space
    pos_near = pos_near.unproject(view_->camera()->viewMatrix(), view_->camera()->projectionMatrix(),
                                  view_->defaultFrameGraph()->viewportRect().toRect());
    pos_far = pos_far.unproject(view_->camera()->viewMatrix(), view_->camera()->projectionMatrix(),
                                view_->defaultFrameGraph()->viewportRect().toRect());
    // join into line
    return point_from_qvec(pos_near) & point_from_qvec(pos_far);
  }

  void MainWindow::scene_part_added(const Part* part) {
    // create part entity
    auto* entity = new PartEntity{part, scene_root_};
    QObject::connect(&editor_, &SceneEditor::selected_part_changed, entity,
                     [entity](const Part* part) { entity->set_selected(entity->part() == part); });
    QObject::connect(&editor_, &SceneEditor::part_transformed, entity, [entity](const Part* part) {
      if (part == entity->part()) {
        entity->update_transform();
      }
    });
    // update outline
    outline_->addItem(QString::fromStdString(part->name()));
  }

  void MainWindow::part_clicked(QPickEvent* event) {
    auto* part_entity = qobject_cast<PartEntity*>(event->entity());
    if (part_entity == nullptr) {
      return;
    }
    if (event->button() == QPickEvent::LeftButton) {
      // select part
      editor_.set_selected_part(part_entity->part());
    }
  }

  void MainWindow::part_mouse_pressed(Qt3DRender::QPickEvent* event) {
    auto* part_entity = qobject_cast<PartEntity*>(event->entity());
    if (part_entity == nullptr) {
      return;
    }
    if (part_entity->part() != editor_.selected_part()) {
      return;
    }

    if (event->button() == QPickEvent::LeftButton) {
      // start moving a part
      const auto drag_mode = Flags{event->modifiers()}.is_set(QPickEvent::ShiftModifier)
                                 ? SceneEditor::DragMode::along_vertical_axis
                                 : SceneEditor::DragMode::along_ground_plane;
      const auto start_point = point_from_qvec(event->worldIntersection());
      editor_.start_dragging(start_point, drag_mode);
    }
  }

  void MainWindow::part_mouse_moved(QPickEvent* event) {
    auto* part_entity = qobject_cast<PartEntity*>(event->entity());
    if (part_entity == nullptr) {
      return;
    }
    if (part_entity->part() != editor_.selected_part()) {
      return;
    }
    if (Flags{event->buttons()}.is_set(QPickEvent::LeftButton)) {
      const auto target_point = point_from_qvec(event->worldIntersection());
      editor_.drag_selected(target_point);
    }
  }

  void MainWindow::part_mouse_released(Qt3DRender::QPickEvent* event) {
    auto* part_entity = qobject_cast<PartEntity*>(event->entity());
    if (part_entity == nullptr) {
      return;
    }
    if (part_entity->part() != editor_.selected_part()) {
      return;
    }
    editor_.commit_dragging();
  }
}
