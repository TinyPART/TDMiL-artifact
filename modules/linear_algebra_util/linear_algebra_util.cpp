

#include "linear_algebra_util.h"
#include <vector>
// #include <iostream>

std::vector<float> LinearAlgebraUtil::subtract_vector(std::vector<float> v1, std::vector<float> v2)
{
    std::vector<float> result(v1.size());
    for (unsigned int i = 0; i < v1.size(); i++)
    {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

std::vector<float>
LinearAlgebraUtil::multiply_matrix_vector(std::vector<std::vector<float>> mat, std::vector<float> v)
{
    std::vector<float> result(mat.size(), 0.0);
    for (unsigned int i = 0; i < mat.size(); i++)
    {
        for (unsigned int j = 0; j < mat[0].size(); j++)
        {
            result[i] += mat[i][j] * v[j];
        }
    }
    return result;
}

std::vector<float> LinearAlgebraUtil::add_vector_scalar(std::vector<float> v, float a)
{
    for (unsigned int i = 0; i < v.size(); i++)
    {
        v[i] += a;
    }
    return v;
}

std::vector<float> LinearAlgebraUtil::multiply_vector_scalar(std::vector<float> v, float a)
{
    for (unsigned int i = 0; i < v.size(); i++)
    {
        v[i] *= a;
    }

    return v;
}

std::vector<std::vector<float>> LinearAlgebraUtil::transpose_vector(std::vector<std::vector<float>> v)
{
    std::vector<std::vector<float>> vT(v[0].size(), std::vector<float>(v.size()));
    for (unsigned int i = 0; i < v.size(); i++)
    {
        for (unsigned int j = 0; j < v[0].size(); j++)
        {
            vT[j][i] = v[i][j];
        }
    }

    return vT;
}
