aux_source_directory(hpc/openmp SRCS_HPC_OPENMP)

set(exec "openmp_map_parallel_find")

set(exec_src "${exec}.cc")
add_executable(${exec} ${exec_src})
