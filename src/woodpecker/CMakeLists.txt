add_library(woodpecker STATIC joint.cpp mesh.cpp part.cpp scene.cpp
                              scene_editor.cpp)
add_library(woodpecker::woodpecker ALIAS woodpecker)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.in.hpp
               ${CMAKE_CURRENT_BINARY_DIR}/config.hpp)

target_include_directories(woodpecker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
                                             ${CMAKE_CURRENT_BINARY_DIR}/..)

set_target_properties(woodpecker PROPERTIES AUTOMOC ON)
target_link_libraries(woodpecker PUBLIC fmt::fmt spdlog::spdlog klein::klein
                                        Boost::boost Qt::Core cxx_std_20)
