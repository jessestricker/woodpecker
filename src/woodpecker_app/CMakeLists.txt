add_executable(woodpecker_app main.cpp util/qt.cpp)

set_target_properties(woodpecker_app PROPERTIES AUTOMOC ON)
target_link_libraries(woodpecker_app PRIVATE woodpecker::woodpecker Qt::Widgets)

# Windows only: run Qt deployment tool after build
if(WIN32)
  add_custom_command(
    TARGET woodpecker_app
    POST_BUILD
    COMMAND
      "${QT_INSTALL_DIR}/bin/windeployqt.exe" --no-translations
      --no-compiler-runtime --no-system-d3d-compiler --no-opengl-sw
      "$<TARGET_FILE:woodpecker_app>"
    COMMENT "Deploying Qt shared libraries to build dir"
    VERBATIM)
endif()
