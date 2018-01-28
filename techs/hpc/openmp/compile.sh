#!/bin/bash

src="openmp_for"
#src="openmp_for_concat"
#src="openmp_for_parallel_sum"

g++ -std=c++11 -O3 -Wall -fopenmp ${src}.cc -o exec_${src}
