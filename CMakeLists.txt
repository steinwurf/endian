cmake_minimum_required(VERSION 3.10)
project(endian)

# Define library
add_library(endian INTERFACE)
target_compile_features(endian INTERFACE cxx_std_14)
target_include_directories(endian INTERFACE src/)
add_library(steinwurf::endian ALIAS endian)

# Install headers
install(
  DIRECTORY ./src/endian
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  FILES_MATCHING
  PATTERN *.hpp)

# Is top level project?
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  # Build executables
  add_executable(convert_to_little_endian examples/convert_to_little_endian.cpp)
  target_link_libraries(convert_to_little_endian endian)
endif()
