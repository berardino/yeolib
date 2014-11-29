cmake_minimum_required (VERSION 3.0)
project (yeolib)

option(BUILD_EXAMPLES "Build examples." on)

include_directories(include)
link_directories(/opt/local/lib)

add_subdirectory(yeorenderer)

