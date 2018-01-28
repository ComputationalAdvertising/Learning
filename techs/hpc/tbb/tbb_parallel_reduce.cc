#include <omp.h>
#include <vector>
#include <iostream>
#include "tbb/parallel_reduce.h"
#include "body.h"

double GetTime(void) {
  timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
    return static_cast<double>(ts.tv_sec) + 
      static_cast<double>(ts.tv_nsec) * 1e-9;
  } else {
    return static_cast<double>(time(NULL));
  }
}

int test(int n) {
  int sum = 0;
  for (int i = 0; i < n; ++i) sum += i/10 + i/100 + i/1000;
  return sum;
}

int test_openmp(int n) {
  int sum = 0;
  #pragma omp parallel for reduction(+:sum) num_threads(4)
  for (int i = 0; i < n; ++i) 
    sum += i/10 + i/100 + i/1000;
  return sum;
}

int test_tbb_parallel_reduce(int n) {
  ApplySum as;
  parallel_reduce(tbb::blocked_range<size_t>(0, n, n/16), as, tbb::auto_partitioner());
  return as.sum();
}

int main(int argc, char** argv) {
  int M = 5000;
  int N = 1000000;
  
  tbb::tick_count start_time = tbb::tick_count::now();
  long sum = 0l;
  for (int i = 0; i < M; ++i) {
    sum += test(N+i);
  }
  tbb::tick_count end_time = tbb::tick_count::now();
  printf("sequence time: %g, sum: %ld\n", (end_time - start_time).seconds(), sum);

  start_time = tbb::tick_count::now();
  sum = 0l;
  for (int i = 0; i < M; ++i) {
    sum += test_openmp(N+i);
  }
  end_time = tbb::tick_count::now();
  printf("openmp time: %g, sum: %ld\n", (end_time - start_time).seconds(), sum);

  start_time = tbb::tick_count::now();
  sum = 0l;
  tbb::task_scheduler_init init(tbb::task_scheduler_init::deferred);
  init.initialize(4);
  for (int i = 0; i < M; ++i) {
    sum += test_tbb_parallel_reduce(N+i);
  }
  end_time = tbb::tick_count::now();
  printf("openmp time: %g, sum: %ld\n", (end_time - start_time).seconds(), sum);
  init.terminate();

  return 0;
}
