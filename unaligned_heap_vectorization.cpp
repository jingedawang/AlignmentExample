#include <immintrin.h>
#include <iostream>

class Vector4d {
  using aligned_double4 = __attribute__ ((aligned (32))) double[4];
public:
  Vector4d() {
  }

  Vector4d(double d1, double d2, double d3, double d4) {
    data[0] = d1;
    data[1] = d2;
    data[2] = d3;
    data[3] = d4;
  }

  aligned_double4 data;
};

Vector4d operator+ (const Vector4d& v1, const Vector4d& v2) {
  __m256d data1 = _mm256_load_pd(v1.data);
  __m256d data2 = _mm256_load_pd(v2.data);
  __m256d data3 = _mm256_add_pd(data1, data2);
  Vector4d result;
  _mm256_store_pd(result.data, data3);
  return result;
}

std::ostream& operator<< (std::ostream& o, const Vector4d& v) {
  o << "(" << v.data[0] << ", " << v.data[1] << ", " << v.data[2] << ", " << v.data[3] << ")";
  return o;
}

int main() {
  Vector4d* input1 = new Vector4d{1, 1, 1, 1};
  Vector4d* input2 = new Vector4d{1, 2, 3, 4};

  std::cout << "address of input1: " << input1->data << std::endl;
  std::cout << "address of input2: " << input2->data << std::endl;

  Vector4d result = *input1 + *input2;

  std::cout << result << std::endl;

  return 0;
}