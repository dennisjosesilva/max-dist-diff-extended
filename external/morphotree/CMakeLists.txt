project(morphotree)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(stb)

find_package(OpenMP REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(PROJECT_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
list(REMOVE_ITEM PROJECT_SOURCE ${PROJECT_MAIN})

add_library(morphotree STATIC ${PROJECT_SOURCE})
target_link_libraries(morphotree OpenMP::OpenMP_CXX)

