/*!
 *  Copyright (c) 2016 by Contributors
 *  \file thread_pool.h
 *  \brief thread pool
 *  \author ZhouYong
 */
#ifndef OPENMIT_TOOLS_THREAD_THREAD_POOL_H_
#define OPENMIT_TOOLS_THREAD_THREAD_POOL_H_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <iostream>

namespace mit {
/*! \brief thread pool */
class ThreadPool {
  public:
    /*! \brief constructor and initialize thread number */
    ThreadPool(int size = 2);
    
    /*! \brief destructor */
    ~ThreadPool();

    /*! \brief task function type */
    using Task = std::function<void(void)>;
  
    inline void Shutdown() { this->stop_.store(true); }

    inline void Restart() { this->stop_.store(false); }
  
    template<class F, class... Args>
    auto Commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;

    Task GetOneWork();
  
    void TaskExecutor();
  private:
    /*! \brief thread pool */
    std::vector<std::thread> pools_;
    /*! \brief task queue */
    std::queue<Task> tasks_;
    /*! \brief thread number */
    int thread_num_;
    /*! \brief mutex for sync task */
    std::mutex mu_;
    std::condition_variable cond_;
    /*! \brief whether stop commit. atomic op */
    std::atomic<bool> stop_;
}; 

ThreadPool::ThreadPool(int size): stop_{false} {
  thread_num_ = size < 1 ? 1 : size;
  for (size_t i = 0; i < thread_num_; ++i) {
    pools_.emplace_back(&ThreadPool::TaskExecutor, this);
    // pools_.push_back(std::thread{...});
  }
}

ThreadPool::~ThreadPool() {
  stop_.store(true);
  cond_.notify_all();
  for (std::thread& thread : pools_) {
    thread.join();
  }
}

template<class F, class... Args>
auto ThreadPool::Commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
  if (stop_.load()) { // equals to 'stop == false'
    throw std::runtime_error("task executor have closed commit.");
  }
  using ResType = decltype(f(args...));
  
  auto task = std::make_shared<std::packaged_task<ResType()>>(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
  );

  // add a task to queue 
  {
    std::lock_guard<std::mutex> lk(mu_);
    tasks_.emplace([task]() { (*task)(); });   // push(Task{...});
  }
  cond_.notify_one();

  std::future<ResType> future = task->get_future();
  return future;
}

ThreadPool::Task ThreadPool::GetOneWork() {
  std::unique_lock<std::mutex> lk(mu_);
  cond_.wait(lk, [this]() { return !tasks_.empty(); });  // wait until have task 
  Task task{std::move(tasks_.front())};
  tasks_.pop();
  return task;
}

// Task Scheduler
void ThreadPool::TaskExecutor() {
  while (true) {
    if (!stop_.load()) {
      std::cout << "stop == true. break;" << std::endl;
      break;
    }
  std::unique_lock<std::mutex> lk(mu_);
  cond_.wait(lk, [this]() { return !tasks_.empty(); });  // wait until have task 
  Task task{std::move(tasks_.front())};
  tasks_.pop();
  task();
  }
}

} // namespace mit
#endif // OPENMIT_TOOLS_THREAD_THREAD_POOL_H_
