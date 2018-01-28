#include <pmmintrin.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <omp.h>

int test(int n) {
  int sum = 0;
  for (int i = 0; i < n; ++i) { sum += i; }
  return sum;
}

int test_openmp(int n) {
  int sum = 0;
  #pragma omp parallel for reduction(+:sum)
  for (int i = 0; i < n; ++i) sum += i;
  return sum;
}

double GetTime(void) {
  timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
    return static_cast<double>(ts.tv_sec) + 
      static_cast<double>(ts.tv_nsec) * 1e-9;
  } else {
    return static_cast<double>(time(NULL));
  }
}

int main(int argc, char **argv) {
  int num_procs = omp_get_num_procs();
  printf("[INFO] num_procs: %d\n", num_procs);
  int num_thread = 4;
  if (argv[1]) num_thread = atoi(argv[1]);
  printf("[INFO] num_thread: %d\n", num_thread);
  //omp_set_num_threads(num_thread);
  
  long sum = 0l;
  int N = 100000;

  // [1] simple thread
  double begin_time = GetTime();
  for (int i = 0; i < N; ++i) { sum += test(i); }
  printf("[INFO] total time (without openmp): %lf ms. sum: %ld\n", (GetTime() - begin_time), sum);

  // [2] omp for schedule
  sum = 0l;
  begin_time = GetTime();
  #pragma omp parallel num_threads(num_thread)
  {
    #pragma omp for reduction(+:sum) schedule(static, N/omp_get_num_threads())
    for (int i = 0; i < N; ++i) { sum += test(i); }
  }
  printf("[INFO] total time (with openmp for schedule): %lf ms. sum: %ld\n", (GetTime() - begin_time), sum);
 
  // [3] omp for 
  sum = 0l;
  begin_time = GetTime();
  #pragma omp parallel for reduction(+:sum) num_threads(num_thread)
  for (int i = 0; i < N; ++i) { 
    sum += test(i); 
  }
  printf("[INFO] total time (with openmp for): %lf ms. sum: %ld\n", (GetTime() - begin_time), sum);

  // [4] omp for 
  sum = 0l;
  begin_time = GetTime();
  #pragma omp parallel for reduction(+:sum) num_threads(num_thread)
  for (int i = 0; i < N; ++i) { 
    sum += test_openmp(i); 
  }
  printf("[INFO] total time (with openmp for / openmp for): %lf ms. sum: %ld\n", (GetTime() - begin_time), sum);

  // [5] two loop 
  std::vector<int> vec(N, 1);
  int* data = vec.data();
  sum = 0l;
  begin_time = GetTime();
  for (int i = 0; i < N/10; ++i) {
    for (int j = i+1; j < N; ++j) {
      sum += *(data+j);
    }
  }
  printf("[INFO] total time (two loop single): %lf ms. sum: %ld\n", (GetTime() - begin_time), sum);

  // [6] two loop (multi-thread)
  sum = 0l;
  begin_time = GetTime();
  #pragma omp parallel for reduction(+:sum) num_threads(num_thread)
  for (int i = 0; i < N/10; ++i) {
    for (int j = i+1; j < N; ++j) {
      sum += *(data+j);
    }
  }
  printf("[INFO] total time (two loop omp): %lf ms. sum: %ld\n", (GetTime() - begin_time), sum);

  return 0;
}
