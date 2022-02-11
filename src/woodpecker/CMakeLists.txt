file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")
add_library(woodpecker STATIC ${sources})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.in.hpp
               ${CMAKE_CURRENT_BINARY_DIR}/config.hpp)

target_include_directories(woodpecker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
                                             ${CMAKE_CURRENT_BINARY_DIR}/..)
target_link_libraries(woodpecker PUBLIC spdlog::spdlog klein::klein
                                        Boost::boost)
set_target_properties(woodpecker PROPERTIES CXX_EXTENSIONS OFF)

add_library(woodpecker::woodpecker ALIAS woodpecker)

# compiler-specific
# ##############################################################################

# disable MSVC extensions
target_compile_options(woodpecker
                       PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

# https://github.com/microsoft/STL/issues/1814#issuecomment-845572895
if(MSVC
   AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29.30129
   AND CMAKE_VERSION VERSION_GREATER 3.20.3)
  target_compile_features(woodpecker PUBLIC cxx_std_23)
else()
  target_compile_features(woodpecker PUBLIC cxx_std_20)
endif()
