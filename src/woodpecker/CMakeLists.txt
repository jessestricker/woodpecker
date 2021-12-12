add_library(woodpecker STATIC scene.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.in.hpp
               ${CMAKE_CURRENT_BINARY_DIR}/config.hpp)

target_compile_features(woodpecker PUBLIC cxx_std_20)
target_include_directories(woodpecker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
                                             ${CMAKE_CURRENT_BINARY_DIR}/..)
target_link_libraries(woodpecker PUBLIC spdlog::spdlog klein::klein)

add_library(woodpecker::woodpecker ALIAS woodpecker)
