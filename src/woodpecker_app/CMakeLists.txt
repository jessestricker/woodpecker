file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")
add_executable(woodpecker_app WIN32 ${sources} main.qrc)

set_target_properties(woodpecker_app PROPERTIES AUTOMOC ON AUTORCC ON)
target_link_libraries(woodpecker_app PRIVATE woodpecker Qt::Widgets Qt::3DCore
                                             Qt::3DRender Qt::3DExtras)
