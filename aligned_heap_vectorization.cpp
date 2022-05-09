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

  void* operator new (std::size_t count) {
    void* original = ::operator new(count + 32);
    void* aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) & ~size_t(32 - 1)) + 32);
    *(reinterpret_cast<void**>(aligned) - 1) = original;
    return aligned;
  }

  void operator delete (void* ptr) {
    ::operator delete(*(reinterpret_cast<void**>(ptr) - 1));
  }
  
  int x;
  aligned_double4 data;
  int y, z;
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

  std::cout << "address of input1.x: " << &input1->x << std::endl;
  std::cout << "address of input1: " << input1->data << std::endl;
  std::cout << "address of input1.y: " << &input1->y << std::endl;
  std::cout << "address of input1.z: " << &input1->z << std::endl;

  Vector4d result = *input1 + *input2;

  std::cout << result << std::endl;

  delete input1;
  delete input2;
  return 0;
}
