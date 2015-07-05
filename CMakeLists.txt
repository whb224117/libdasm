cmake_minimum_required(VERSION 2.8)

project(dasm)

set(SOURCES libdasm.c)

add_library(dasm_shared SHARED ${SOURCES})
add_library(dasm_static STATIC ${SOURCES})
set_target_properties(dasm_shared dasm_static PROPERTIES OUTPUT_NAME dasm)


