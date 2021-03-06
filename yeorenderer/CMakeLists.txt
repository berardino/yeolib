project (yeorenderer)

find_package(OpenGL REQUIRED)

pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLFW REQUIRED glfw3 glfw)

add_compile_options(-DGLFW_INCLUDE_GLCOREARB -DGLFW_INCLUDE_GLEXT)
include_directories(include ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
link_directories(/opt/local/lib)

add_library(yeorenderer SHARED src/renderer.cpp)
add_dependencies(yeorenderer yeomath)

target_link_libraries(yeorenderer ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} yeomath)
target_include_directories(yeorenderer PUBLIC "${PROJECT_SOURCE_DIR}/include")

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

