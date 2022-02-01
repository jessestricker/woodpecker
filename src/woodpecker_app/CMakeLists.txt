add_executable(woodpecker_app main.cpp main.qrc matcap_material.cpp util/qt.cpp)

set_target_properties(woodpecker_app PROPERTIES AUTOMOC ON AUTORCC ON)
target_link_libraries(woodpecker_app PRIVATE woodpecker Qt::Widgets Qt::3DCore
                                             Qt::3DRender Qt::3DExtras)
