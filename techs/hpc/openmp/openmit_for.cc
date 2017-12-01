#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

int test(int n) {
  int sum = 0;
  for (int i = 0; i < n; ++i) { sum += i; }
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
  int num_thread = 4;
  if (argv[1]) num_thread = atoi(argv[1]);
  printf("[INFO] num_thread: %d\n", num_thread);
  omp_set_num_threads(num_thread);
  
  long sum = 0l;
  int N = 100000;

  printf("[INFO] single thread, serialize for sum.\n");
  double begin_time = GetTime();
  for (int i = 0; i < N; ++i) { sum += test(i); }
  printf("[INFO] total time (without openmp): %lf ms. sum: %ld\n", (GetTime() - begin_time), sum);

  printf("[INFO] openmp #schedule(static, n)\n");
  sum = 0l;
  begin_time = GetTime();
  #pragma omp parallel 
  {
    #pragma omp for reduction(+:sum)
    for (int i = 0; i < N; ++i) { sum += test(i); }
  }
  printf("[INFO] total time (with openmp schedule): %lf ms. sum: %ld\n", (GetTime() - begin_time), sum);
 

  printf("[INFO] openmp parallel for reduction(+:sum)\n");
  sum = 0l;
  begin_time = GetTime();
  #pragma omp parallel for reduction(+:sum)
  for (int i = 0; i < N; ++i) { 
    sum += test(i); 
  }
  printf("[INFO] total time (with openmp reduction): %lf ms. sum: %ld\n", (GetTime() - begin_time), sum);

  return 0;
}
