cmake_minimum_required(VERSION 3.10)

project(EDT)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP)

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

# Exemplos do Professor Paulo
add_executable(dtmt src/Apps/Misc/dtmt.cpp)
target_link_libraries(dtmt
  gft 
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)


add_executable(dtmt_diff 
  src/MaxDist/edt_diff.cpp
  src/Apps/Misc/dtmt_diff.cpp)

target_link_libraries(dtmt_diff
    gft 
    Threads::Threads
    ZLIB::ZLIB
    OpenMP::OpenMP_CXX)

# Extinction filters
add_executable(extinction_filter_max_dist 
  src/MaxDist/edt_diff.cpp
  src/Apps/Filters/extinction_filter_max_dist.cpp
  src/MaxDist/MaxDistComputer.cpp)

target_link_libraries(extinction_filter_max_dist
  morphotree
  gft 
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

add_executable(extinction_filter_area 
  src/Apps/Filters/extinction_filter_area.cpp
  src/MaxDist/edt_diff.cpp
  src/MaxDist/MaxDistComputer.cpp)

target_link_libraries(extinction_filter_area
  morphotree
  gft 
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)


add_executable(extinction_filter_volume 
  src/MaxDist/edt_diff.cpp
  src/Apps/Filters/extinction_filter_volume.cpp
  src/MaxDist/MaxDistComputer.cpp)

target_link_libraries(extinction_filter_volume
  morphotree
  gft 
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

add_executable(extinction_filter_height
  src/MaxDist/edt_diff.cpp 
  src/Apps/Filters/extinction_filter_height.cpp
  src/MaxDist/MaxDistComputer.cpp)

target_link_libraries(extinction_filter_height
  morphotree
  gft 
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

add_executable(extinction_filter_inertia 
  src/Apps/Filters/extinction_filter_inertia.cpp
  src/Attributes/InertiaComputer.cpp)

target_link_libraries(extinction_filter_inertia
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

# display maximum distance attribute.
add_executable(max_dist_attr_disp 
  src/MaxDist/MaxDistComputer.cpp
  src/MaxDist/edt_diff.cpp
  src/Apps/Displays/max_dist_attr_disp.cpp)
  
target_link_libraries(max_dist_attr_disp
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

add_executable(ultimate_maxDist_open
src/MaxDist/edt_diff.cpp 
src/MaxDist/MaxDistComputer.cpp
src/Apps/Filters/ultimate_maxDist_open.cpp)

target_link_libraries(ultimate_maxDist_open
morphotree
gft 
Threads::Threads
ZLIB::ZLIB
OpenMP::OpenMP_CXX)


# Direct filter
add_executable(direct_filter 
  src/Attributes/BasicAttributeComputer.cpp
  src/Apps/Filters/direct_filter.cpp)
  
target_link_libraries(direct_filter
  morphotree  
  OpenMP::OpenMP_CXX)

# basic attr display
add_executable(basic_attr_disp 
  src/Attributes/BasicAttributeComputer.cpp
  src/Apps/Displays/basic_attr_disp.cpp)
  
target_link_libraries(basic_attr_disp
  morphotree  
  OpenMP::OpenMP_CXX)  


# runtime computations
add_executable(diff_max_dist_runtime
  src/MaxDist/edt_diff.cpp 
  src/MaxDist/MaxDistComputer.cpp
  src/Apps/Runtime/diff_max_dist_runtime.cpp)
  
target_link_libraries(diff_max_dist_runtime
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

# runtime computations
add_executable(non_diff_max_dist_runtime 
  src/MaxDist/NonDiffMaxDistComputer.cpp
  src/Apps/Runtime/non_diff_max_dist_runtime.cpp)
  
target_link_libraries(non_diff_max_dist_runtime
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)

# Checking
add_executable(check_implementations
  src/MaxDist/edt_diff.cpp 
  src/MaxDist/NonDiffMaxDistComputer.cpp
  src/MaxDist/MaxDistComputer.cpp
  src/Apps/Checking/check_implementations.cpp)
  
target_link_libraries(check_implementations
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)


# test_temp 
add_executable(check_contour_opt
  src/Apps/Checking/check_contour_opt.cpp
  src/Morphology/Adjacency.cpp
  src/Morphology/ErosionSerial.cpp
  src/Contour/IncrementalContourSerial.cpp)

  target_link_libraries(check_contour_opt
    morphotree
    gft
    Threads::Threads
    ZLIB::ZLIB
    OpenMP::OpenMP_CXX)


# max_dist_opt checking
add_executable(check_diff_edt_opt
  src/Apps/Checking/check_diff_edt_opt.cpp
  src/Morphology/Adjacency.cpp
  src/Morphology/ErosionSerial.cpp
  src/Contour/IncrementalContourSerial.cpp
  src/MaxDist/edt_diff.cpp
  src/MaxDist/MaxDistComputerOpt.cpp
  src/MaxDist/MaxDistComputer.cpp)

target_link_libraries(check_diff_edt_opt
  morphotree
  gft
  Threads::Threads
  ZLIB::ZLIB
  OpenMP::OpenMP_CXX)