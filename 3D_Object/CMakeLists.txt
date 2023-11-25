cmake_minimum_required(VERSION 3.10)
project(3D_Object)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include include)

add_executable(3D_Object src/main.cpp src/Model.cpp)

find_package(glfw3 REQUIRED)
target_link_libraries(3D_Object glfw ${CMKAE_DL_LIBS})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(3D_Object ${OPENGL_gl_LIBRARY} GL GLU ${GLUT_LIBRARIES})