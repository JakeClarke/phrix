set(DXSDK_DIR $ENV{DXSDK_DIR})

include_directories (${SDL2_INCLUDE_DIR} ${DXSDK_DIR}/Include)

add_library(directx directx.cc directx.h d3dx12.h dxresourcefactory.h dxresourcefactory.cc)
target_link_libraries(directx "${SDL2_LIBRARY}" D3D12.lib Dxgi.lib)