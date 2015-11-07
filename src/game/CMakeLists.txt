include_directories (${SDL2_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}graphics/)

add_library(game game.cc game.h jobmanager.cc jobmanager.h scene.cc scene.h servicemanager.cc servicemanager.h)
target_link_libraries(game "${SDL2_LIBRARY}")