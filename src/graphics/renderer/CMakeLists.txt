if(WIN32)
  SET(RENDERERS 
   directx
  )
endif(WIN32)


add_subdirectory (${RENDERERS})

add_library(renderer renderer.cc renderer.h)

target_link_libraries(renderer ${RENDERERS})