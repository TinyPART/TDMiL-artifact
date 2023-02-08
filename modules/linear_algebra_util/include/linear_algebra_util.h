

#ifndef FLOWER_CPPV2_LINEAR_ALGEBRA_UTIL_H
#define FLOWER_CPPV2_LINEAR_ALGEBRA_UTIL_H

#include <vector>

class LinearAlgebraUtil {
 public:
  static std::vector<float> subtract_vector(std::vector<float> v1, std::vector<float> v2);

  static std::vector<float> multiply_matrix_vector(std::vector<std::vector<float>> mat, std::vector<float> v);

  static std::vector<float> add_vector_scalar(std::vector<float> v, float a);

  static std::vector<float> multiply_vector_scalar(std::vector<float> v, float a);

  static std::vector<std::vector<float>> transpose_vector(std::vector<std::vector<float>> v);

};

#endif //FLOWER_CPPV2_LINEAR_ALGEBRA_UTIL_H
