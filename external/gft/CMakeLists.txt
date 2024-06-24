project(gft)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

SET(PROJECT_SOURCE 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_common.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_adjrel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_image32.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_image32f.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_image64.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_cimage.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_cimage32f.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_morphology.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_morphology3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_queue.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_gpqueue_by_Falcao.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_graph.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_graphclustering.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_imagegraph.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_imagegraphpx.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_layeredgraph.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_heap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_heap32.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_heap64f.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_heap_lex.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_heap32fi_lex.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_heap32fif_lex.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_heap32fiif_lex.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_heappair.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_ift.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_string.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_arraylist.cpp  
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_filelist.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_highlight.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_color.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_superpixels.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_evaluation.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_marker.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_scene.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_scene32.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_scene64.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_scene32f.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_scene64f.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_scene16.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_scene8.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_adjrel3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_scnmath.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_matrix.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_bmap.cpp  
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_set.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_stack.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_analysis.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_analysis3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_pqueue32.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_pqueue16.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_curve.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_radiometric.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_radiometric3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_filtering.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_filtering3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_adjregion3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_seedmap3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_segmentation3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_gradient3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_features.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_attributelist.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_configfile.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_geometry.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_ctree.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_analysis3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_llw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_band.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_dicom.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gft_bzlib.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/znzlib.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/nifti1_io.c)

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

add_library(gft STATIC ${PROJECT_SOURCE})
