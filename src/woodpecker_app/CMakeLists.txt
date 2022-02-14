add_executable(
  woodpecker_app WIN32
  main.cpp
  main.qrc
  main_window.cpp
  mesh_renderer.cpp
  part_entity.cpp
  part_material.cpp
  util/qt.cpp)

set_target_properties(woodpecker_app PROPERTIES AUTOMOC ON AUTORCC ON)
target_link_libraries(
  woodpecker_app PRIVATE woodpecker Qt::Widgets Qt::3DCore Qt::3DRender
                         Qt::3DExtras cxx_std_20)
