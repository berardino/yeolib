project (yeoio)

pkg_search_module(collada REQUIRED collada-dom)
include_directories(include ${collada_INCLUDE_DIRS} ${collada_INCLUDE_DIRS}/1.5)

add_library(yeoio SHARED src/io.cpp)

#set_target_properties(yeoio PROPERTIES COMPILE_FLAGS "${collada_CFLAGS_OTHER}") 


target_link_libraries(yeoio ${collada_LIBRARIES} boost_system-mt freeimage yeorenderer)
target_include_directories(yeoio PUBLIC "${PROJECT_SOURCE_DIR}/include")


