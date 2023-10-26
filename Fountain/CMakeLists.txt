cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)
project(Fountain)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

add_executable(Fountain src/main.cpp)

find_package(glfw3 REQUIRED)
target_link_libraries(Fountain glfw ${CMKAE_DL_LIBS})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(Fountain ${OPENGL_gl_LIBRARY} GL GLU ${GLUT_LIBRARIES})