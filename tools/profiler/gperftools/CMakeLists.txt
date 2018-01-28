set(exec "gperftools_basic")

set(exec_src "${exec}.cc")
add_executable(${exec} ${exec_src})
target_link_libraries(${exec} profiler)
