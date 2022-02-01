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
CPMAddPackage("gh:gabime/spdlog@1.9.2")
CPMAddPackage("gh:doctest/doctest#2.4.7")
CPMAddPackage("gh:jeremyong/klein#16f46a0ad3c843beea7b13ea666658ca475ec665")

# targets
add_subdirectory(src/woodpecker)
add_subdirectory(src/woodpecker_app)
