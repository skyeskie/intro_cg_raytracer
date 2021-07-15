cmake_minimum_required(VERSION 3.10)

# Prevend in-source builds
# .gitignore should handle, but just make sure
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR “In-source build detected!”)
endif()

project(assign2)
find_package(GLUT REQUIRED)
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(mod_base)
add_subdirectory(mod_dummy)
add_subdirectory(mod_tiff)

add_executable(ipr main.cpp)

target_link_libraries(ipr
  mod_base
  mod_dummy
  mod_tiff
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
  ${GLUT_LIBRARY}
  ${X11_LIBRARIES}
)

set_target_properties(ipr PROPERTIES COMPILE_FLAGS "-g -O2 -Wall -fomit-frame-pointer -ffast-math -fexpensive-optimizations -D_REENTRANT")

file(COPY images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
