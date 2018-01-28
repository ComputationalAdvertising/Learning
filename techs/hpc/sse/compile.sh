#!/bin/bash

src="sse_array_vars_product"
#src="sse_test"
#src="sse_array_sum"
#src="sse_mul_sum"

g++ -std=c++11 -O3 -Wall -msse4 -fopenmp ${src}.cc -o exec_${src}
