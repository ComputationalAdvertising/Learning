#!/bin/bash

src="vector_insert_pushback_emplaceback"

g++ -std=c++11 -O3 -Wall -msse4 -fopenmp ${src}.cc -o exec_${src}
