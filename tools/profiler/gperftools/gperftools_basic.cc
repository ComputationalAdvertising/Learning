#include "gperftools/profiler.h"
#include <stdio.h>
#include <unistd.h>

void test() {
  int sum = 0;
  for (int i = 0; i < 1; ++i) {
    sum += i % 10;
  }
}

void pull(long N) {
  int sum = 0;
  for (long i = 0; i < N * 10000000; ++i) {
    sum += i + i/10 + i/20;
  }
}

void push(long N) {
  int sum = 0;
  for (long i = 0; i < N * 10000000; ++i) {
    sum += i + i/10;
    //test();
  }
}

void control() {
  for (long i = 0; i < 1000000000; ++i) {
    //printf("i: %d\n", i);
    pull(i);
    push(i);
    //if (i % 1000000 == 0) { printf("i: %ld\n", i); }
  }
}

int main(int argc, char** argv) {
  ProfilerStart("basic.prof");
  control();
  //sleep(1);
  ProfilerStop();
  return 0;
}
