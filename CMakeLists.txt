cmake_minimum_required (VERSION 3.10)
project(endian)

add_library(endian INTERFACE)
add_library(steinwurf::endian ALIAS endian)

target_include_directories(endian INTERFACE src/)
