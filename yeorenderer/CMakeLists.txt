cmake_minimum_required (VERSION 3.0)
project (yeorenderer)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-option")

option(BUILD_EXAMPLES "Build examples." on)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(include ${GLFW_INCLUDE_DIRS})
link_directories(/opt/local/lib)

add_library(yeorenderer SHARED src/renderer.cpp)

target_link_libraries(yeorenderer ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

