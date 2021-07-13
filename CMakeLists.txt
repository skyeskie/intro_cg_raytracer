cmake_minimum_required(VERSION 2.8)

project(assign1)
find_package(GLUT REQUIRED)
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(mod_base)
add_subdirectory(mod_dummy)

add_executable(assign1 mod_dummy/DummyHandle.cpp mod_base/FunctionCommand.cpp main.cpp mod_base/CommandHandle.cpp)

target_link_libraries(assign1 ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLUT_LIBRARY} ${X11_LIBRARIES})

set_target_properties(assign1 PROPERTIES COMPILE_FLAGS "-g -O2 -Wall -fomit-frame-pointer -ffast-math -fexpensive-optimizations -D_REENTRANT")

