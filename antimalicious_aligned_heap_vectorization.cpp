#include <immintrin.h>
#include <iostream>

#define ALIGNED_OPERATOR_NEW \
  void* operator new (std::size_t count) { \
    void* original = ::operator new(count + 32); \
    void* aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) & ~size_t(32 - 1)) + 32); \
    *(reinterpret_cast<void**>(aligned) - 1) = original; \
    return aligned;\
  } \
  void operator delete (void* ptr) { \
    ::operator delete(*(reinterpret_cast<void**>(ptr) - 1)); \
  }

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

  ALIGNED_OPERATOR_NEW

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

class Point {
public:
  Point(Vector4d position) : position(position) {
  }

  ALIGNED_OPERATOR_NEW

  Vector4d position;
};

int main() {
  Vector4d* input1 = new Vector4d{1, 1, 1, 1};
  Vector4d* input2 = new Vector4d{1, 2, 3, 4};

  Point* point1 = new Point{*input1};
  Point* point2 = new Point{*input2};

  std::cout << "address of point1: " << point1->position.data << std::endl;
  std::cout << "address of point2: " << point2->position.data << std::endl;

  Vector4d result = point1->position + point2->position;

  std::cout << result << std::endl;

  delete input1;
  delete input2;
  delete point1;
  delete point2;
  return 0;
}
