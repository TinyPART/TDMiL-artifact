
#include "synthetic_dataset.h"

#include <algorithm>
#include <cmath>
#include <random>
// #include <iostream>

SyntheticDataset::SyntheticDataset(std::vector<float> ms, float b, size_t size)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> distr(-10.0, 10.0);
    puts("True parameters: \n");
    for (unsigned int i = 0; i < ms.size(); i++)
    {
        // std::cout << std::fixed << "  m" << i << " = " << ms[i] << std::endl;
        puts(" inside dataset");
        printf("true wts %f \n", ms[i]);
    }
    printf(" true bias %f \n", b);
    // std::cout << "  b = " << std::fixed << b << std::endl;

    std::vector<std::vector<float>> xs(size, std::vector<float>(ms.size()));
    std::vector<float> ys(size, 0);
    for (unsigned int m_ind = 0; m_ind < ms.size(); m_ind++)
    {
        std::uniform_real_distribution<float> distx(-10.0, 10.0);

        for (unsigned int i = 0; i < size; i++)
        {
            xs[i][m_ind] = distx(mt);
        }
    }

    for (unsigned int i = 0; i < size; i++)
    {
        ys[i] = b;
        for (unsigned int m_ind = 0; m_ind < ms.size(); m_ind++)
        {
            ys[i] += ms[m_ind] * xs[i][m_ind];
        }
    }

    std::vector<std::vector<float>> data_points;
    for (size_t i = 0; i < size; i++)
    {
        std::vector<float> data_point;
        data_point.insert(data_point.end(), xs[i].begin(), xs[i].end());
        data_point.push_back(ys[i]);

        data_points.push_back(data_point);
    }

    this->data_points = data_points;
}

size_t SyntheticDataset::size()
{
    return this->data_points.size();
}

int SyntheticDataset::get_features_count()
{
    return this->data_points[0].size() - 1;
}

std::vector<std::vector<float>> SyntheticDataset::get_data_points()
{
    return this->data_points;
}
