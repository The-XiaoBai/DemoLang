cmake_minimum_required(VERSION 3.10)

project(DemoLang VERSION 1.0.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(include)

add_subdirectory(src ${CMAKE_BINARY_DIR}/src)


option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    add_subdirectory(tests ${CMAKE_BINARY_DIR}/tests)
endif()