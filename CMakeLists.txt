cmake_minimum_required(VERSION 3.2)

project(Learning)
find_package(openmp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3 -fPIC -Wall -mssse3")

include_directories(
  ${PROJECT_SOURCE_DIR}
  /home/zhouyongsdzh/software/gperftools-2.6.3_build/include
)

link_directories(
  ${PROJECT_SOURCE_DIR}/lib
  /home/zhouyongsdzh/software/gperftools-2.6.3_build/lib
)

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

#add_subdirectory(techs)
add_subdirectory(tools/profiler/gperftools)
