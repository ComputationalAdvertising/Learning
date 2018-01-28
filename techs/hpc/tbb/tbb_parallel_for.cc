#include <iostream>
#include "tbb/parallel_for.h"
#include "body.h"
using namespace tbb;

void test(int n) {
  for (int i = 0; i < n; ++i) {
    std::cout << "i: " << i << std::endl;
  }
}

void test_tbb_for(int n) {
  /*
  // [1] parallel_for(first, last, [step], fun);  step default 1
  parallel_for(0, n, [](int i) { std::cout << "tbb-for i: " << i << std::endl; });
  // [2] step: step size, not chuck size
  parallel_for(0, n, n/10, [](int i) { std::cout << "tbb-for i: " << i << std::endl; });
  // [3] lambda
  parallel_for(blocked_range<size_t>(0, n, n), [=](blocked_range<size_t>& r) {
    for (size_t i = r.begin(); i != r.end(); ++i) {
      std::cout << "tbb-for i: " << i << std::endl;
    }
  });
  */
  // [4] functor 
  parallel_for(tbb::blocked_range<size_t>(0, n, n/10), ApplyFor());
}

int main(int argc, char** argv) {
  int N = 1000;
  test(N);
  test_tbb_for(N);
  return 0;
}
