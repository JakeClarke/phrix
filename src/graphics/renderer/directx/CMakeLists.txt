set(DXSDK_DIR $ENV{DXSDK_DIR})

include_directories (${SDL2_INCLUDE_DIR} ${DXSDK_DIR}/Include)

add_library(directx directx.cc directx.h)
target_link_libraries(directx "${SDL2_LIBRARY}" D3D11.lib)