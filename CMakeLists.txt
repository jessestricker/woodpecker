cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(
  woodpecker
  VERSION 0.1.0
  LANGUAGES CXX)

# extra properties
set(PROJECT_DISPLAY_NAME "Woodpecker")
set(PROJECT_AUTHOR "Jesse Stricker")

# dependencies: Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets 3DCore 3DRender 3DInput 3DExtras)
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
  GIT_TAG "9e16beeb13e1e5ecb46be5cae37c545ab18f25b1"
  GITHUB_REPOSITORY "jessestricker/klein")
CPMAddPackage(
  NAME "Catch2"
  VERSION "3.0.0-preview4"
  GIT_TAG "v3.0.0-preview4"
  GITHUB_REPOSITORY "catchorg/Catch2")

# targets
add_subdirectory(src/woodpecker)
add_subdirectory(src/woodpecker_app)

# sane defaults for standard C++
# ##############################################################################

# interface library exporting usage requirements
add_library(cxx_std_20 INTERFACE)

set_property(TARGET cxx_std_20 PROPERTY INTERFACE_CXX_EXTENSIONS OFF)

# https://github.com/microsoft/STL/issues/1814#issuecomment-845572895
target_compile_features(
  cxx_std_20 INTERFACE $<IF:$<CXX_COMPILER_ID:MSVC>,cxx_std_23,cxx_std_20>)

# enable 'conformance mode' for MSVC
target_compile_options(
  cxx_std_20
  INTERFACE
    "$<$<CXX_COMPILER_ID:MSVC>:/permissive-;/Zc:__cplusplus;/Zc:preprocessor;/D_MSVC_LANG=202004L>"
)
