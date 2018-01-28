#include "tbb/concurrent_unordered_map.h"

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

typedef tbb::concurrent_unordered_map<long, Node*> table_type;

int main(int argc, char** argv) {
  table_type table;
  long N = 100000;
  printf("insert\n");
  for (long i = 0; i < N; ++i) {
    table.insert(std::make_pair(i, new Node(i, "node." + std::to_string(i))));
  }
  printf("find\n");
  for (long i = 0; i < N; ++i) {
    if (i % 999 == 0 && table.find(i) != table.end()) {
      printf("i: %ld, node content: %s\n", i, table.at(i)->DebugString().c_str());
    } 
  }
  return 0;
}
