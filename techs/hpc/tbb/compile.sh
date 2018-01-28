#!/bin/bash

#src="tbb_parallel_for"
#src="tbb_parallel_reduce"
#src="tbb_concurrent_hash_map"
src="tbb_concurrent_unordered_map"

#g++ -O3 -Wall -msse4 -fopenmp -I/usr/local/include -L/usr/lib -std=c++11 ${src}.cc -o exec_${src} -ltbbmalloc -ltbb -lrt

g++ -O3 -Wall -msse4 -fopenmp -std=c++11 ${src}.cc -o exec_${src} -ltbb -lrt

#g++ -O3 -DNDEBUG -D_CONSOLE  -o ${src} ${src}.cc -ltbb -lrt
