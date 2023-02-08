//
// Created by Andreea Zaharia on 01/02/2022.
//

#ifndef FLOWER_CPP_LINE_FIT_MODEL_H
#define FLOWER_CPP_LINE_FIT_MODEL_H

#include <vector>

#include "synthetic_dataset.h"
#include "linear_algebra_util.h"

class LineFitModel
{
public:
  LineFitModel(unsigned int num_iterations, float learning_rate, unsigned int num_params);

  std::vector<float> predict(std::vector<std::vector<float>> X);

  std::tuple<std::size_t, float, float> train_SGD(SyntheticDataset &dataset);

  std::tuple<float, float> train_SGD2(SyntheticDataset &dataset);

  std::tuple<std::size_t, float, float> evaluate(SyntheticDataset &test_dataset);

  std::vector<float> get_pred_weights();

  void set_pred_weights(std::vector<float> new_pred_weights);

  float get_bias();

  void set_bias(float new_bias);

  std::size_t get_model_size();

private:
  unsigned int num_iterations;
  unsigned int batch_size;
  float learning_rate;

  std::vector<float> pred_weights;
  float pred_b;

  float compute_mse(std::vector<float> true_y, std::vector<float> pred);
};

#endif // FLOWER_CPP_LINE_FIT_MODEL_H
