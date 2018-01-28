#!/bin/bash

src="thread_pool_test"

g++ -std=c++11 -O3 -Wall -msse4 -fopenmp ${src}.cc -o exec_${src} -lpthread
