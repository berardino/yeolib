cmake_minimum_required (VERSION 3.0)
project (yeomath)

find_package(Armadillo REQUIRED)
include_directories(include ${ARMADILLO_INCLUDE_DIRS})

add_library(yeomath SHARED src/math.cpp)

target_link_libraries(yeomath ${ARMADILLO_LIBRARIES})

