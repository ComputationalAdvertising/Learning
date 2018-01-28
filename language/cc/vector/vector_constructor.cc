#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  std::vector<float> vec(100);
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] = i;
  }
  std::vector<float> vec1(vec.data() + 10, vec.data() + 10 + 4);
  std::cout << "vec1.size: " << vec1.size() << std::endl;
  for (size_t i = 0; i < vec1.size(); ++i) {
    std::cout << "i: " << i << ", elem: " << vec1[i] << std::endl;
  }
  return 0;
}
