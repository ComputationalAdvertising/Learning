#include <iostream>
#include <vector>
#include <pmmintrin.h>

double GetTime(void) {
  timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
    return static_cast<double>(ts.tv_sec) + 
      static_cast<double>(ts.tv_nsec) * 1e-9;
  } else {
    return static_cast<double>(time(NULL));
  }
}

void vec_insert(std::vector<int>& vec, std::vector<int>& target) {
  vec.insert(vec.end(), target.data(), target.data() + target.size());
}

void vec_push_back(std::vector<int>& vec, std::vector<int>& target) {
  for (auto i = 0u; i < target.size(); ++i) { 
    vec.push_back(target[i]); 
  }
}

void vec_emplace_back(std::vector<int>& vec, std::vector<int>& target) {
  for (auto i = 0u; i < target.size(); ++i) {
    vec.emplace_back(target[i]);
  }
}

int main(int argc, char** argv) {
  int N = 300000;
  int M = 100000;
  std::vector<int> vec;
  std::vector<int> target(M);
  for (int i = 0; i < M; ++i) {
    target[i] = i;
  }

  for (int i = 0; i < 0; ++i) {
    std::cout << "i: " << i << std::endl;
  }

  double start = GetTime();
  for (int i = 0; i < N; ++i) {
    vec.clear();
    vec_insert(vec, target);
  }
  std::cout << "test_insert: " << (GetTime() - start) << ", vec[208]: " << vec[208] << ", size: " << vec.size() << std::endl;
  
  start = GetTime();
  vec.clear(); vec.resize(0);
  for (int i = 0; i < N; ++i) {
    vec.clear();
    vec_push_back(vec, target);
  }
  std::cout << "test_push_back: " << (GetTime() - start) << ", vec[208]: " << vec[208] << ", size: " << vec.size() << std::endl;

  start = GetTime();
  vec.clear(); vec.resize(0);
  for (int i = 0; i < N; ++i) {
    vec.clear();
    vec_emplace_back(vec, target);
  }
  std::cout << "test_emplace_back: " << (GetTime() - start) << ", vec[208]: " << vec[208] << ", size: " << vec.size() << std::endl;

  return 0;
}
