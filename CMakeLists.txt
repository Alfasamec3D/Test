CMake_minimum_required(VERSION 3.5)

project(triangle)

set(SOURCE_EXE main.cpp)

include_directories(geom)

add_executable(main ${SOURCE_EXE})

add_subdirectory(geom)

target_link_libraries(main geom)