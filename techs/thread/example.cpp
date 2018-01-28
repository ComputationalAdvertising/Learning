#include <iostream>
#include <vector>
#include <chrono>

#include "thread_pool.h"

void test(int i) {
  printf("I-%d\n", i);
}

int main()
{
    
  mit::ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 10000; ++i) {
      //pool.Append([i]() { test(i); });
      //pool.enqueue([i]() { test(i); });
      /*
      results.emplace_back(
        pool.enqueue([i]() { test(i); return i; })
      );
      */
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                //std::this_thread::sleep_for(std::chrono::seconds(1));
                //std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }
    /*
    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    */
    return 0;
}
