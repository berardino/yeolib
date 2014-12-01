project (yeorenderer)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(include ${GLFW_INCLUDE_DIRS} ../yeomath/include)
link_directories(/opt/local/lib)

add_library(yeorenderer SHARED src/renderer.cpp)
add_dependencies(yeorenderer yeomath)

target_link_libraries(yeorenderer ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

