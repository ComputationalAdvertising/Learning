//#include <map>
#include <unordered_map>
#include <iostream>

int main(int argc, char** argv) {
  int N = 10;
  //std::map<int, int> maps;
  std::unordered_map<int, int> maps;
  for (int i = N; i > 0; --i) {
    maps.insert(std::make_pair(i, N - i));
  }

  for (auto iter = maps.begin(); iter != maps.end(); ++iter) {
    std::cout << "key: " << iter->first << ", value: " << iter->second << std::endl;
  }

  return 0;
}
