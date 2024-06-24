cmake_minimum_required(VERSION 3.10)

project(EDT)

set(CMAKE_CXX_STANDARD 14)

if(APPLE)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang\$")
        set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp")
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang\$")
        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp")
    endif()

endif()

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/external/gft/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/external/morphotree/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/external/stb"
  "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/gft")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/morphotree")

find_package(OpenMP REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

# Exemplos do professor Paulo
add_executable(dtmt src/dtmt.cpp)
target_link_libraries(dtmt
  gft 
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)


add_executable(dtmt_diff src/dtmt_diff.cpp)
target_link_libraries(dtmt_diff
    gft 
    Threads::Threads
    ZLIB::ZLIB
    OpenMP::OpenMP_CXX)

# Extinction filters
add_executable(extinction_filter_max_dist 
  src/extinction_filter_max_dist.cpp
  src/MaxDistComputer.cpp)

target_link_libraries(extinction_filter_max_dist
  morphotree
  gft 
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

add_executable(extinction_filter_area 
  src/extinction_filter_area.cpp
  src/MaxDistComputer.cpp)

target_link_libraries(extinction_filter_area
  morphotree
  gft 
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)


add_executable(extinction_filter_volume 
  src/extinction_filter_volume.cpp
  src/MaxDistComputer.cpp)

target_link_libraries(extinction_filter_volume
  morphotree
  gft 
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

add_executable(extinction_filter_height 
  src/extinction_filter_height.cpp
  src/MaxDistComputer.cpp)

target_link_libraries(extinction_filter_height
  morphotree
  gft 
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

add_executable(extinction_filter_inertia 
  src/extinction_filter_inertia.cpp
  src/InertiaComputer.cpp)

target_link_libraries(extinction_filter_inertia
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

# display maximum distance attribute.
add_executable(max_dist_attr_disp 
  src/MaxDistComputer.cpp
  src/max_dist_attr_disp.cpp)
  
target_link_libraries(max_dist_attr_disp
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

add_executable(ultimate_maxDist_open 
src/MaxDistComputer.cpp
src/ultimate_maxDist_open.cpp)

target_link_libraries(ultimate_maxDist_open
morphotree
gft 
Threads::Threads
ZLIB::ZLIB
OpenMP::OpenMP_CXX)


# Direct filter
add_executable(direct_filter 
  src/BasicAttributeComputer.cpp
  src/direct_filter.cpp)
  
target_link_libraries(direct_filter
  morphotree  
  OpenMP::OpenMP_CXX)

# basic attr display
add_executable(basic_attr_disp 
  src/BasicAttributeComputer.cpp
  src/basic_attr_disp.cpp)
  
target_link_libraries(basic_attr_disp
  morphotree  
  OpenMP::OpenMP_CXX)  


# runtime computations
add_executable(diff_max_dist_runtime 
  src/MaxDistComputer.cpp
  src/diff_max_dist_runtime.cpp)
  
target_link_libraries(diff_max_dist_runtime
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

# runtime computations
add_executable(non_diff_max_dist_runtime 
  src/NonDiffMaxDistComputer.cpp
  src/non_diff_max_dist_runtime.cpp)
  
target_link_libraries(non_diff_max_dist_runtime
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

# Checking
add_executable(check_implementations 
  src/NonDiffMaxDistComputer.cpp
  src/MaxDistComputer.cpp
  src/check_implementations.cpp)
  
target_link_libraries(check_implementations
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)