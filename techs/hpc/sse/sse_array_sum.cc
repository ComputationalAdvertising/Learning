#include <omp.h>
#include <pmmintrin.h>
#include <stdio.h>
#include <time.h>
#include <vector>

double GetTime(void) {
  timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
    return static_cast<double>(ts.tv_sec) + 
      static_cast<double>(ts.tv_nsec) * 1e-9;
  } else {
    return static_cast<double>(time(NULL));
  }
}

// version 1
float sum1(const float * val, unsigned int size) {
  float sum = 0;
  for (unsigned int i = 0; i < size; ++i) { sum += val[i]; }
  return sum;
}

// version 2
float sum2(const float * val, unsigned int size) {
  float sum = 0;
  int nBlockWidth = 4;  // SSE
  int cntBlock = size / nBlockWidth;
  int cntMem = size % nBlockWidth;
  __m128 fSum = _mm_setzero_ps();
  __m128 fLoad;

  const float * p = val;
  for (int i = 0; i < cntBlock; ++i) {
    fLoad = _mm_load_ps(p);
    fSum = _mm_add_ps(fSum, fLoad);
    p += nBlockWidth;
  }
  const float *q = (const float *)&fSum;
  sum = q[0] + q[1] + q[2] + q[3];
  for (int i = 0; i < cntMem; ++i) { sum += p[i]; }
  return sum;
}

// version 3
float sumfloat_sse_4loop(const float* val, unsigned int size) {
  float sum = 0;
  int nBlockWidth = 4 * 4;
  int cntBlock = size / nBlockWidth;
  int cntMem = size % nBlockWidth;
  __m128 fSum0 = _mm_setzero_ps();
  __m128 fSum1 = _mm_setzero_ps();
  __m128 fSum2 = _mm_setzero_ps();
  __m128 fSum3 = _mm_setzero_ps();
  __m128 fLoad0, fLoad1, fLoad2, fLoad3;
  const float* p = val;
  for (int i = 0; i < cntBlock; ++i) {
    // load 
    fLoad0 = _mm_load_ps(p);
    fLoad1 = _mm_load_ps(p+4);
    fLoad2 = _mm_load_ps(p+8);
    fLoad3 = _mm_load_ps(p+12);
    // add 
    fSum0 = _mm_add_ps(fSum0, fLoad0);
    fSum1 = _mm_add_ps(fSum1, fLoad1);
    fSum2 = _mm_add_ps(fSum2, fLoad2);
    fSum3 = _mm_add_ps(fSum3, fLoad3);
    p += nBlockWidth;
  }
  fSum0 = _mm_add_ps(fSum0, fSum1);
  fSum2 = _mm_add_ps(fSum2, fSum3);
  fSum0 = _mm_add_ps(fSum0, fSum2);
  const float* q = (const float*)&fSum0;
  sum = q[0] + q[1] + q[2] + q[3];
  for (int i = 0; i < cntMem; ++i) { sum += p[i]; }
  return sum;
}

float sumfloat_sse_4loop_v2(const float* val, unsigned int size) {
  float sum = 0;
  int cntBlock = size / 4;
  __m128* p = (__m128*) val;
  __m128 fSum0 = _mm_setzero_ps();
  __m128 fSum1 = _mm_setzero_ps();
  __m128 fSum2 = _mm_setzero_ps();
  __m128 fSum3 = _mm_setzero_ps();
  //__m128 fLoad0, fLoad1, fLoad2, fLoad3;
  for (int i = 0; i < cntBlock; i += 4) {
    fSum0 = _mm_add_ps(fSum0, *p);
    fSum1 = _mm_add_ps(fSum1, *(++p));
    fSum2 = _mm_add_ps(fSum2, *(++p));
    fSum3 = _mm_add_ps(fSum3, *(++p));
    ++p;
  }
  fSum0 = _mm_add_ps(fSum0, fSum1);
  fSum2 = _mm_add_ps(fSum2, fSum3);
  fSum0 = _mm_add_ps(fSum0, fSum2);
  const float* q = (const float*)&fSum0;
  sum = q[0] + q[1] + q[2] + q[3];
  for (unsigned int i = 0; i < size % 16; ++i) { sum += val[size-1-i]; }
  return sum;
}

int main(int argc, char** argv) {
  int num_thread = 4;
  if (argc > 1) num_thread = atoi(argv[1]);
  printf("[INFO] num_thread: %d\n", num_thread);
  omp_set_num_threads(num_thread);

  int N = 1000;
  int M = 4096*4096;
  std::vector<float> vec(M);
  for (int i = 0; i < M; ++i) { vec[i] = 1.0 * i; }
  
  std::vector<float> arr(N);
  double begin_time = GetTime();
  
  printf("[INFO] array for accmulation sum (without sse)\n");
  for (int i = 0; i < N; ++i) {
    arr[i] = sum1(vec.data(), vec.size());
  }
  printf("[INFO] array (without sse): %lf s. arr[99]: %f\n", (GetTime() - begin_time), arr[99]);

  begin_time = GetTime();
  arr.clear(); arr.resize(N);
  for (int i = 0; i < N; ++i) {
    arr[i] = sum2(vec.data(), vec.size());
  }
  printf("[INFO] array sum (with sse): %lf s. arr[99]: %f\n", (GetTime() - begin_time), arr[99]);
  
  begin_time = GetTime();
  arr.clear(); arr.resize(N);
  int chunksize = N / num_thread;
  chunksize = N % num_thread == 0 ? chunksize : chunksize + 1;
  #pragma omp parallel for schedule(static, chunksize)
  for (int i = 0; i < N; ++i) {
    arr[i] = sum2(vec.data(), vec.size());
  }
  printf("[INFO] array sum (with openmp and sse): %lf s. arr[99]: %f\n", (GetTime() - begin_time), arr[99]);
  
  begin_time = GetTime();
  arr.clear(); arr.resize(N);
  #pragma omp parallel for schedule(static, chunksize)
  for (int i = 0; i < N; ++i) {
    arr[i] = sumfloat_sse_4loop(vec.data(), vec.size());
  }
  printf("[INFO] array sum (with openmp and sse-4loop): %lf s. arr[99]: %f\n", (GetTime() - begin_time), arr[99]);
  
  begin_time = GetTime();
  arr.clear(); arr.resize(N);
  #pragma omp parallel for schedule(static, chunksize)
  for (int i = 0; i < N; ++i) {
    arr[i] = sumfloat_sse_4loop_v2(vec.data(), vec.size());
  }
  printf("[INFO] array sum (with openmp and sse-4loop-v2): %lf s. arr[99]: %f\n", (GetTime() - begin_time), arr[99]);
  return 0;
}
