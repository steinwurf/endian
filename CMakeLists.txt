cmake_minimum_required(VERSION 3.10)
project(sw_endian)


# Use waf to resolve dependencies
if(NOT DEFINED STEINWURF_RESOLVE)
  message(STATUS "Resolving dependencies...")
  execute_process(
    COMMAND ${Python_EXECUTABLE} waf resolve ${STEINWURF_RESOLVE_OPTIONS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE STATUS)

  if(STATUS AND NOT STATUS EQUAL 0)
    message(FATAL_ERROR "Failed: ${STATUS}")
  endif()

  set(STEINWURF_RESOLVE "${CMAKE_CURRENT_SOURCE_DIR}/resolve_symlinks")
endif()


# Include common CMake settings
include("${STEINWURF_RESOLVE}/toolchains/common_settings.cmake")


# Define library
add_library(sw_endian INTERFACE)
target_compile_features(sw_endian INTERFACE cxx_std_14)
target_include_directories(sw_endian INTERFACE src/)
add_library(steinwurf::endian ALIAS sw_endian)

# Install headers
install(
  DIRECTORY ./src/endian
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  FILES_MATCHING
  PATTERN *.hpp)

# Is top level project?
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})

    # Include gtest
    if(NOT TARGET steinwurf::gtest)
      add_subdirectory("${STEINWURF_RESOLVE}/gtest" EXCLUDE_FROM_ALL)
    endif()

    file(GLOB_RECURSE endian_test_sources test/**.cpp)
    add_executable(sw_endian_tests ${endian_test_sources})
    target_link_libraries(sw_endian_tests ${steinwurf_object_libraries}
                          steinwurf::gtest steinwurf::endian)

    enable_testing()
    add_test(NAME sw_endian_tests COMMAND sw_endian_tests)


    # Convert to little endian
    add_executable(sw_endian_example_convert_to_little_endian
                   examples/convert_to_little_endian.cpp)
    target_link_libraries(sw_endian_example_convert_to_little_endian
                          ${steinwurf_object_libraries} steinwurf::endian)

    # Stream writer reader
    add_executable(sw_endian_example_stream_writer_reader
                   examples/stream_writer_reader.cpp)
    target_link_libraries(sw_endian_example_stream_writer_reader
                          ${steinwurf_object_libraries} steinwurf::endian)

    # Network
    add_executable(sw_endian_example_network examples/network.cpp)
    target_link_libraries(sw_endian_example_network ${steinwurf_object_libraries}
                          steinwurf::endian)

endif()
