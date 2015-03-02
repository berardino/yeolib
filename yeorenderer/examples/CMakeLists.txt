add_executable (simple simple.cpp)

add_compile_options(-std=c++11 -stdlib=libc++)
target_link_libraries(simple yeorenderer yeomath yeoio)

