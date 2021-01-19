cmake_minimum_required (VERSION 3.10)
project(endian)

file(GLOB endian_headers ./src/endian/*.hpp)

add_library(endian INTERFACE)
add_library(steinwurf::endian ALIAS endian)

target_include_directories(endian INTERFACE src/)

install(FILES ${endian_headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/endian)
