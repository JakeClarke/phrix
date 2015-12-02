add_subdirectory (renderer)
add_library(graphics graphics.cc graphics.h graphicsscene.h
texture.h texture2d.h color.h shader.h pixelshader.h vertexshader.h graphicsresourcefactory.h)

include_directories (${PROJECT_SOURCE_DIR}/game/)

target_link_libraries(graphics renderer)