//
// Created by Andreea Zaharia on 31/01/2022.
//

#ifndef FLOWER_CPP_SYNTHETIC_DATASET_H
#define FLOWER_CPP_SYNTHETIC_DATASET_H

#include <vector>

class SyntheticDataset
{
public:
  // Generates the synthetic dataset of size size around given vector m of size ms_size and given bias b.
  SyntheticDataset(std::vector<float> ms, float b, std::size_t size);

  // Returns the size of the dataset.
  std::size_t size();

  // Returns the dataset.
  std::vector<std::vector<float>> get_data_points();

  int get_features_count();

private:
  std::vector<float> ms;
  float b;

  // The label is the last position in the vector.
  // TODO: consider changing this to a pair with the label.

  std::vector<std::vector<float>> data_points;
};

#endif // FLOWER_CPP_SYNTHETIC_DATASET_H
