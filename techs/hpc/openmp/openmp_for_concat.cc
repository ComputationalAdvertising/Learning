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
  
  int N = 100;
  int M = 30000001;
  int mapsize = 300000;
  std::vector<int> vec;
  
  std::unordered_map<int, int> map;
  for (int i = 0; i < mapsize; ++i) { map[i] = i*10; }
  printf("[INFO] map.size: %ld\n", map.size());

  // simple thread
  printf("[INFO] single thread, serialize for fill \n");
  double begin_time = GetTime();
  for (int i = 0; i < N; ++i) {
    vec.clear(); vec.resize(M);
    for (int j = 0; j < M; ++j) { vec[j] = map[j % mapsize]; }
  }
  printf("test vec.size: %d\n", (int) vec.size());
  printf("[INFO] total time (without openmp): %lf s\n", (GetTime() - begin_time));
  fprintf(stdout, "vec.size: %d, vec[9999]: %d\n", (int)vec.size(), vec[9999]);
  
  // omp for concat schedule
  vec.clear();
  printf("[INFO] openmp #schedule(static, n)\n");
  begin_time = GetTime();
  std::vector<std::vector<int> *> threads_vec(num_thread);
  // initial threads vector
  for (size_t i = 0; i < threads_vec.size(); ++i) {
    threads_vec[i] = new std::vector<int>();
  }
  // for parallel
  int blocksize = M % num_thread == 0 ? M / num_thread : (M / num_thread) + 1;
  for (int i = 0; i < N; ++i) {
    for (size_t k = 0; k < threads_vec.size(); ++k) {
      threads_vec[k]->clear();
      size_t thread_vec_size = k < threads_vec.size() - 1 ? blocksize : M - blocksize * k;
      threads_vec[k]->resize(thread_vec_size);
    }
    #pragma omp parallel for schedule(static, blocksize)
    for (int j = 1; j < M; ++j) {
      (*threads_vec[omp_get_thread_num()])[j % blocksize] = map[j % mapsize];
    }
  }
  // for concat
  for (size_t i = 0; i < threads_vec.size(); ++i) {
    printf("ID: %d, size: %d\n", (int)i, (int)threads_vec[i]->size());
    vec.insert(vec.end(), threads_vec[i]->begin(), threads_vec[i]->end());
  }
  printf("[INFO] total time (with openmp schedule): %lf s\n", (GetTime() - begin_time));
  fprintf(stdout, "vec.size: %d, vec[9999]: %d\n", (int)vec.size(), vec[9999]);

  return 0;
}
