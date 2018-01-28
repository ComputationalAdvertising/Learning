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

float array_multi(const float* v1, const float* v2, size_t size) {
  float result = 0.0f;
  for (size_t i = 0; i < size; ++i) {
    result += v1[i] * v2[i];
  }
  return result;
}

float array_multi_sse(const float* v1, const float* v2, size_t size) {
  float result = 0.0f;
  __m128* sv1 = (__m128*)v1;
  __m128* sv2 = (__m128*)v2;
  __m128 fMul = _mm_setzero_ps();
  int cntBlock = size / 4;
  for (int i = 0; i < cntBlock; ++i) {
    fMul = _mm_add_ps(fMul, _mm_mul_ps(*(sv1++), *(sv2++)));
  }
  const float* q = (const float*)&fMul;
  result = q[0] + q[1] + q[2] + q[3];
  for (size_t i = 0; i < size % 4; ++i) { 
    result += v1[size-1-i] * v2[size-1-i]; 
  }
  return result;
}

float array_multi_sse_4loop(const float* v1, const float* v2, size_t size) {
  float result = 0.0f;
  int cntBlock = size / 4;
  __m128* sv1 = (__m128*)v1;
  __m128* sv2 = (__m128*)v2;
  __m128 fMul0 = _mm_setzero_ps();
  __m128 fMul1 = _mm_setzero_ps();
  __m128 fMul2 = _mm_setzero_ps();
  __m128 fMul3 = _mm_setzero_ps();
  for (int i = 0; i < cntBlock; i += 4) {
    fMul0 = _mm_add_ps(fMul0, _mm_mul_ps(*sv1, *sv2));
    fMul1 = _mm_add_ps(fMul0, _mm_mul_ps(*(++sv1), *(++sv2)));
    fMul2 = _mm_add_ps(fMul0, _mm_mul_ps(*(++sv1), *(++sv2)));
    fMul3 = _mm_add_ps(fMul0, _mm_mul_ps(*(++sv1), *(++sv2)));
    ++sv1; ++sv2;
  }
  fMul0 = _mm_add_ps(fMul0, fMul1);
  fMul2 = _mm_add_ps(fMul2, fMul3);
  fMul0 = _mm_add_ps(fMul0, fMul2);
  const float* q = (const float*)&fMul0;
  result = q[0] + q[1] + q[2] + q[3];
  for (size_t i = 0; i < size % 16; ++i) {
    result += v1[size-1-i] * v2[size-1-i];
  }
  return result;
}

int main(int argc, char** argv) {
  int num_thread = 4;
  if (argc > 1) num_thread = atoi(argv[1]);
  printf("[INFO] num_thread: %d\n", num_thread);
  omp_set_num_threads(num_thread);

  int N = 1000000;
  int M = 4096;
  std::vector<float> v1(M);
  std::vector<float> v2(M);
  for (int i = 0; i < M; ++i) { v1[i] = 1.0*i; v2[i] = 1.0*i; }
  
  std::vector<float> arr(N);
  double begin_time = GetTime();
  for (int i = 0; i < N; ++i) {
    arr[i] = array_multi(v1.data(), v2.data(), v1.size());
  }
  printf("[INFO] array (without sse): %lf s. arr[99]: %f\n", (GetTime() - begin_time), arr[99]);

  begin_time = GetTime();
  arr.clear(); arr.resize(N);
  for (int i = 0; i < N; ++i) {
    arr[i] = array_multi_sse(v1.data(), v2.data(), v1.size());
  }
  printf("[INFO] array (with sse): %lf s. arr[99]: %f\n", (GetTime() - begin_time), arr[99]);

  begin_time = GetTime();
  arr.clear(); arr.resize(N);
  #pragma omp parallel for num_threads(num_thread)
  for (int i = 0; i < N; ++i) {
    arr[i] = array_multi_sse(v1.data(), v2.data(), v1.size());
  }
  printf("[INFO] array (with openmp + sse): %lf s. arr[99]: %f\n", (GetTime() - begin_time), arr[99]);

  begin_time = GetTime();
  arr.clear(); arr.resize(N);
  #pragma omp parallel for num_threads(num_thread)
  for (int i = 0; i < N; ++i) {
    arr[i] = array_multi_sse_4loop(v1.data(), v2.data(), v1.size());
  }
  printf("[INFO] array (with openmp + sse-4loop): %lf s. arr[99]: %f\n", (GetTime() - begin_time), arr[99]);

  return 0;
} 
