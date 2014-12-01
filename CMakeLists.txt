cmake_minimum_required (VERSION 2.8)
project (yeolib)

find_package(PkgConfig REQUIRED)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${yeolib_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${yeolib_BINARY_DIR}/bin")

option(BUILD_EXAMPLES "Build examples." on)
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(include /opt/local/include)
link_directories(/opt/local/lib)

add_subdirectory(yeomath)
add_subdirectory(yeorenderer)
add_subdirectory(yeoio)

