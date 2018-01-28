#include <unistd.h>
#include <vector>
#include <pmmintrin.h>
#include <iostream>
#include <omp.h>

int embedding_size_ = 4;

double GetTime(void) {
  timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
    return static_cast<double>(ts.tv_sec) + 
      static_cast<double>(ts.tv_nsec) * 1e-9;
  } else {
    return static_cast<double>(time(NULL));
  }
}

float sum(const float* v, size_t size) {
  float val = 0.0f;
  for (auto offset = 1u; offset < size; ++offset) {
    val += v[offset] * v[offset + 5];
  }
  return val;
}

float sum_sse(const float* v, size_t size) {
  __m128 inprod = _mm_setzero_ps();
  auto offset = 1u;
  for (; offset < size; offset += 4) {
    __m128 v1 = _mm_loadu_ps(v + offset);
    __m128 v2 = _mm_loadu_ps(v + offset + 5);
     inprod = _mm_add_ps(inprod, _mm_mul_ps(v1, v2));
  }
  inprod = _mm_hadd_ps(inprod, inprod);
  inprod = _mm_hadd_ps(inprod, inprod);
  float sum;
  _mm_store_ss(&sum, inprod);
  return sum;
}

int main(int argc, char ** argv) {
  omp_set_num_threads(4);
  long count = std::atol(argv[1]);
  printf("count: %ld\n",count);
  std::vector<float> a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  a.push_back(5);
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  a.push_back(5);
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  a.push_back(5);
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  a.push_back(5);
  int embedding_size = 4;
  const float * vifj = a.data();
  
  double start_time = GetTime();
  double wTx = 0.0;
  auto size = a.size()-7;
  printf("size: %d\n", size);
  for (long i = 0; i < count; ++i) {
    wTx += sum(vifj, size);
  }
  printf("time consuming (without sse): %lf, wTx: %f\n", (GetTime() - start_time), wTx);

  start_time = GetTime();
  __m128 inprod = _mm_setzero_ps();
  wTx = 0.0f;
  auto cntBlock = 1;
  #pragma omp parallel for reduction(+:wTx) num_threads(4) 
  for (long i = 0; i < count; ++i) {
    wTx += sum_sse(vifj, size);
  }
  printf("time consuming (with sse): %lf, wTx: %f, sum: %d\n", (GetTime() - start_time), wTx);
  return 0;
}
