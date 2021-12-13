cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(
  woodpecker
  VERSION 0.1.0
  LANGUAGES CXX)

# extra properties
set(PROJECT_DISPLAY_NAME "Woodpecker")
set(PROJECT_AUTHOR "Jesse Stricker")

# dependencies: Qt
include(cmake/QtEnv.cmake)
list(APPEND CMAKE_PREFIX_PATH "${QT_INSTALL_DIR}")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# dependencies: CPM
include(cmake/CPM.cmake)
CPMAddPackage("gh:gabime/spdlog@1.9.2")
CPMAddPackage("gh:doctest/doctest#2.4.7")
CPMAddPackage("gh:jeremyong/klein@2.2.1")

# targets
add_subdirectory(src/woodpecker)
add_subdirectory(src/woodpecker_app)
