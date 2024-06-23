cmake_minimum_required(VERSION 3.10)
project(ppos-test)

set(SOURCE_FILES 
    ppos-core-aux.c 
    disk.c
    pingpong-disco1.c
    ppos_disk.c
)

add_executable(ppos-test ${SOURCE_FILES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/disk.dat ${CMAKE_CURRENT_BINARY_DIR}/disk.dat COPYONLY)

set(LIB_PPOS_STATIC ${CMAKE_CURRENT_SOURCE_DIR}/libppos_static.a)

target_link_libraries(ppos-test ${LIB_PPOS_STATIC})