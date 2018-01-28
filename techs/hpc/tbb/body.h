#ifndef TBB_BODY_H_
#define TBB_BODY_H_

#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/tick_count.h"

/*! 
 * \brief body object for tbb::parallel_for
 */
class ApplyFor {
public:
  void operator()(const tbb::blocked_range<size_t>& r) const {
    for (size_t i = r.begin(); i != r.end(); ++i) {
      std::cout << "functor i: " << i << std::endl;
    }
  }

  ApplyFor() : arr_(NULL) {}

  ApplyFor(float arr[]) : arr_(arr) {}

private:
  const float* arr_;
};

/*! 
 * \brief body object for tbb::parallel_reduce (sum op)
 */
class ApplySum {
public:

  void operator()(const tbb::blocked_range<size_t>& r) {
    const float* a = arr_;
    int local_sum = global_sum;
    size_t end = r.end();
    for (size_t i = r.begin(); i != end; ++i) {
      local_sum += i/10 + i/100 + i/1000;
      //local_sum += i;
    }
    global_sum = local_sum;
  }

  ApplySum(ApplySum& x, tbb::split) : arr_(x.arr_), global_sum(0) {}

  void join(const ApplySum& y) { global_sum += y.global_sum; }

  ApplySum(float arr[]) : arr_(arr), global_sum(0) {}

  ApplySum() : arr_(NULL), global_sum(0) {}

  int sum() { return global_sum; }

private:
  const float* arr_;
  int global_sum = 0;
};

#endif // TBB_BODY_H_
