#include <string>
#include "tbb/concurrent_hash_map.h"
#include "tbb/tick_count.h"

class Node {
  public:
    Node() : id_(0), name_("") {}
    Node(int id, std::string name) : id_(id), name_(name) {}
    std::string DebugString() {
      std::string res("id_:");
      res += std::to_string(id_) + ", name:" + name_;
      return res;
    }
  private:
    long id_;
    std::string name_;
};

int main(int argc, char** argv) {
  tbb::concurrent_hash_map<long, Node*> table;
  tbb::concurrent_hash_map<long, Node*>::accessor ac;
  long N = 100000;
  printf("insert\n");
  for (long i = 0; i < N; ++i) {
    table.insert(ac, i);
    ac->second = new Node(i, "node." + std::to_string(i));
  }
  printf("find\n");
  for (long i = 0; i < N; ++i) {
    if (i % 999 == 0 && table.find(ac, i)) {
      printf("i: %ld, node content: %s\n", i, ac->second->DebugString().c_str());
    } 
  }
}
