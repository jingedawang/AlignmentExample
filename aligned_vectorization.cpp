#include <immintrin.h>
#include <iostream>

int main() {

  __attribute__ ((aligned (32))) double input1[4] = {1, 1, 1, 1};
  __attribute__ ((aligned (32))) double input2[4] = {1, 2, 3, 4};
  __attribute__ ((aligned (32))) double result[4];

  std::cout << "address of input1: " << input1 << std::endl;
  std::cout << "address of input2: " << input2 << std::endl;

  __m256d a = _mm256_load_pd(input1);
  __m256d b = _mm256_load_pd(input2);
  __m256d c = _mm256_add_pd(a, b);

  _mm256_store_pd(result, c);

  std::cout << result[0] << " " << result[1] << " " << result[2] << " " << result[3] << std::endl;

  return 0;
}