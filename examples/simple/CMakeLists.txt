cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

project(simple)

find_package(DASM REQUIRED)
    if(NOT DASM_FOUND)
        message(FATAL_ERROR "DASM library not found")
    endif(NOT DASM_FOUND)
include_directories(${DASM_INCLUDE_DIR})

set(SOURCES main.c)

add_executable(simple ${SOURCES})
target_link_libraries(simple ${SDL2_LIBRARY} ${DASM_LIBRARY})



