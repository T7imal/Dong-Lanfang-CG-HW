cmake_minimum_required(VERSION 3.10)
project(Scan_Line)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

add_executable(Scan_Line src/main.cpp)

find_package(glfw3 REQUIRED)
target_link_libraries(Scan_Line glfw ${CMKAE_DL_LIBS})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(Scan_Line ${OPENGL_gl_LIBRARY} GL GLU ${GLUT_LIBRARIES})