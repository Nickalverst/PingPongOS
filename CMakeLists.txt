cmake_minimum_required(VERSION 3.10)
project(ppos-test)

set(SOURCE_FILES 
    ppos-core-aux.c 
    # pingpong-preempcao.c
    pingpong-scheduler-srtf.c
    # pingpong-contab-prio.c
)

add_executable(ppos-test ${SOURCE_FILES})

set(LIB_PPOS_STATIC ${CMAKE_CURRENT_SOURCE_DIR}/libppos_static.a)

target_link_libraries(ppos-test ${LIB_PPOS_STATIC})
