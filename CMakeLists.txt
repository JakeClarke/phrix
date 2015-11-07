cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project (phrix CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

MESSAGE(STATUS "Module path: " ${CMAKE_MODULE_PATH})

find_package(SDL2 REQUIRED)

include_directories ("${PROJECT_SOURCE_DIR}/src/game")
add_subdirectory (src/game)
add_subdirectory (src/graphics)

add_executable(phrix src/phrix.cc)
target_compile_features(phrix PRIVATE cxx_lambdas)
target_link_libraries (phrix game graphics)