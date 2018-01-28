#include <pmmintrin.h>
#include <vector>
#include <cstdio>

void GradEmbeddingWithSSE(std::vector<float>& weight, float xij_middle, std::vector<float>* grad, size_t offset1 = 0, size_t offset2 = 0) {
  __m128 mMiddle = _mm_set1_ps(xij_middle);

  int nBlockWidth = 4;
  int cntBlock = weight.size() / 4;
  int remainder = weight.size() % 4;
  const float* pweight = weight.data();
  __m128 mWeight;
  __m128 mRes;
  int offset = 0;
  for (; offset < cntBlock * 4; offset += 4) {
    mWeight = _mm_loadu_ps(pweight + offset);
    mRes = _mm_mul_ps(mWeight, mMiddle);
    const float* q = (const float*)&mRes;
    for (int j = 0; j < nBlockWidth; ++j) {
      (*grad)[offset + j] = q[j]; 
    } 
  }

  for (int i = 0; i < remainder; ++i) {
    (*grad)[offset + i] = weight[offset + i] * xij_middle;
    printf("(*grad)[%d]: %f\n", offset+i, (*grad)[offset+i]);
  }
}


int main(int argc, char** argv) {
  int size =11;
  std::vector<float> grad;
  std::vector<float> weight(size);
  for (int i = 0; i < size; ++i) { weight[i] = i+1; }
  float xi = 0.5;
  float xj = 2;
  float loss_grad = 0.5;
  float instweight = 1;
  grad.resize(size);
  auto xij_middle = xi * xj * loss_grad * instweight;
  GradEmbedingWithSSE(weight, xij_middle, &grad);

  for (int i = 0; i < size; ++i) {
    printf("grad[%d]: %f\n", i, grad[i]);
  }
  return 0;
}
