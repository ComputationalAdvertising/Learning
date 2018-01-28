#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <vector>
#include <time.h>
#include <unordered_map>

double GetTime(void) {
  timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
    return static_cast<double>(ts.tv_sec) + 
      static_cast<double>(ts.tv_nsec) * 1e-9;
  } else {
    return static_cast<double>(time(NULL));
  }
}

int main(int argc, char** argv) {
  int num_thread = 4;
  if (argv[1]) num_thread = atoi(argv[1]);
  printf("[INFO] num_thread: %d\n", num_thread);
  omp_set_num_threads(num_thread);

  int N = 10000;
  int M = 1000000;
  std::vector<int> vec(M, 0);

  // simple thread 
  printf("[INFO] single thread, serialize for fill\n");
  double begin_time = GetTime();
  for (int i = 1; i < N; ++i) {
    for (int j = 0; j < M; ++j) { vec[j] += (i*i*i)/(i*i); }
  }
  printf("[INFO] total time (without openmp): %lf s, vec[10001]: %d\n", (GetTime() - begin_time), vec[10001]);

  // omp parallel for 
  begin_time = GetTime();
  std::vector<std::vector<int>*> threads_vec(num_thread);
  // initialize threads vector 
  for (size_t i = 0; i < threads_vec.size(); ++i) {
    threads_vec[i] = new std::vector<int>(M);
  }
  int blocksize = N % num_thread == 0 ? N / num_thread : (N / num_thread) + 1;
  #pragma omp parallel for schedule(static, blocksize)
  for (int i = 1; i < N; ++i) {
    int tid = omp_get_thread_num();
    for (int j = 0; j < M; ++j) { (*threads_vec[tid])[j] += (i*i*i) / (i*i); }
  }
  // parallel for sum
  vec.clear(); vec.resize(M);
  blocksize = M % blocksize ? M / num_thread : (M / num_thread) + 1;
  #pragma omp parallel for schedule(static, blocksize)
  for (int i = 0; i < M; ++i) {
    for (int tid = 0; tid < num_thread; ++tid) {
      vec[i] += (*threads_vec[tid])[i];
    }
  }
  for (int tid = 0; tid < num_thread; ++tid) {
    if (threads_vec[tid]) { delete threads_vec[tid]; threads_vec[tid] = NULL; }
  }
  printf("[INFO] total time (openmp): %lf s, vec[10001]: %d\n", (GetTime() - begin_time), vec[10001]);
  return 0;
}
