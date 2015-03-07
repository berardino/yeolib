cmake_minimum_required (VERSION 3.1.3)
project (yeolib)

find_package(PkgConfig REQUIRED)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${yeolib_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${yeolib_BINARY_DIR}/bin")

option(BUILD_EXAMPLES "Build examples." on)
add_compile_options(-std=c++11 -stdlib=libc++)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(include /opt/local/include)
link_directories(/opt/local/lib)

add_subdirectory(yeomath)
add_subdirectory(yeorenderer)
add_subdirectory(yeoio)

