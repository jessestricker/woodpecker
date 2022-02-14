cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(
  woodpecker
  VERSION 0.1.0
  LANGUAGES CXX)

# extra properties
set(PROJECT_DISPLAY_NAME "Woodpecker")
set(PROJECT_AUTHOR "Jesse Stricker")

# dependencies: Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets 3DCore 3DRender 3DExtras)
message(STATUS "Using Qt version: ${Qt6_VERSION}")

# dependencies: Boost
find_package(Boost 1.56 REQUIRED)
message(STATUS "Using Boost version: ${Boost_VERSION_STRING}")

# dependencies: CPM
include(cmake/CPM.cmake)
CPMAddPackage(
  NAME "fmt"
  GIT_TAG "8.1.1"
  GITHUB_REPOSITORY "fmtlib/fmt")
CPMAddPackage(
  NAME "spdlog"
  GIT_TAG "v1.9.2"
  GITHUB_REPOSITORY "gabime/spdlog"
  OPTIONS "SPDLOG_FMT_EXTERNAL ON")
CPMAddPackage(
  NAME "klein"
  VERSION "2.2.2-alpha"
  GIT_TAG "16f46a0ad3c843beea7b13ea666658ca475ec665"
  GITHUB_REPOSITORY "jeremyong/klein")

# targets
add_subdirectory(src/woodpecker)
add_subdirectory(src/woodpecker_app)
