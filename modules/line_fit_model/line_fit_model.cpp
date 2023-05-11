

#include "line_fit_model.h"
#include "synthetic_dataset.h"

#include <vector>
#include <algorithm>
#include <random>
#include <tuple>
LineFitModel::LineFitModel(unsigned int num_iterations, float learning_rate, unsigned int num_params)
    : num_iterations(num_iterations), learning_rate(learning_rate)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> distr(-10.0, 10.0);
    for (unsigned int i = 0; i < num_params; i++)
    {
        this->pred_weights.push_back(distr(mt));
    }

    this->pred_b = 0.0;
    this->batch_size = 8;
}
std::vector<float> LineFitModel::get_pred_weights()
{
    std::vector<float> copy_of_weights(this->pred_weights);
    return copy_of_weights;
}

void LineFitModel::set_pred_weights(std::vector<float> new_weights)
{
    this->pred_weights.assign(new_weights.begin(), new_weights.end());
}

float LineFitModel::get_bias()
{
    return this->pred_b;
}
void LineFitModel::set_bias(float new_bias)
{
    this->pred_b = new_bias;
}

size_t LineFitModel::get_model_size()
{
    return this->pred_weights.size();
};

std::vector<float> LineFitModel::predict(std::vector<std::vector<float>> X)
{
    std::vector<float> prediction(X.size(), 0.0);
    for (unsigned int i = 0; i < X.size(); i++)
    {
        for (unsigned int j = 0; j < X[i].size(); j++)
        {
            prediction[i] += this->pred_weights[j] * X[i][j];
        }
        prediction[i] += this->pred_b;
    }

    return prediction;
}

std::tuple<size_t, float, float, float, float> LineFitModel::train_SGD(SyntheticDataset &dataset)
{
    int features = dataset.get_features_count();
    std::vector<std::vector<float>> data_points = dataset.get_data_points();

    std::vector<float> data_indices(dataset.size());
    for (std::size_t i = 0; i < dataset.size(); i++)
    {
        data_indices.push_back(i);
    }

    std::vector<float> dW(features);
    std::vector<float> err(batch_size, 10000);
    std::vector<float> pW(features);
    float training_error = 0.0;
    for (unsigned int iteration = 0; iteration < num_iterations; iteration++)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(data_indices.begin(), data_indices.end(), g);

        std::vector<std::vector<float>> X(this->batch_size, std::vector<float>(features));
        std::vector<float> y(this->batch_size);

        for (unsigned int i = 0; i < this->batch_size; i++)
        {
            std::vector<float> point = data_points[data_indices[i]];
            y[i] = point.back();
            point.pop_back();
            X[i] = point;
        }

        pW = this->pred_weights;
        float pB = this->pred_b;
        float dB;

        std::vector<float> pred = predict(X);

        err = LinearAlgebraUtil::subtract_vector(y, pred);

        dW = LinearAlgebraUtil::multiply_matrix_vector(LinearAlgebraUtil::transpose_vector(X), err);
        dW = LinearAlgebraUtil::multiply_vector_scalar(dW, (-2.0 / this->batch_size));

        dB = (-2.0 / this->batch_size) * std::accumulate(err.begin(), err.end(), 0.0);

        this->pred_weights =
            LinearAlgebraUtil::subtract_vector(pW, LinearAlgebraUtil::multiply_vector_scalar(dW, learning_rate));
        this->pred_b = pB - learning_rate * dB;

        if (iteration % 100 == 0)
        {
            training_error = this->compute_mse(y, predict(X));
            printf("Iteration:%d and Training error: %f \n", iteration, training_error);
        }
    }
    puts("Local model:\n");
    for (size_t i = 0; i < pred_weights.size(); i++)
    {
        printf("m%d_local = %f \n", i, pred_weights[i]);
    }
    printf("b_local= %f \n \n", pred_b);
    // float accuracy = 1 / training_error;
    return std::make_tuple(dataset.size(), pred_weights[0], pred_weights[1], pred_weights[2], pred_b);
}

float LineFitModel::compute_mse(std::vector<float> true_y, std::vector<float> pred)
{
    float error = 0.0;

    for (unsigned int i = 0; i < true_y.size(); i++)
    {
        error += (pred[i] - true_y[i]) * (pred[i] - true_y[i]);
    }

    return error / (1.0 * true_y.size());
}

std::tuple<size_t, float, float> LineFitModel::evaluate(SyntheticDataset &test_dataset)
{
    std::vector<std::vector<float>> data_points = test_dataset.get_data_points();
    int num_features = data_points[0].size();
    std::vector<std::vector<float>> X(test_dataset.size(), std::vector<float>(num_features));
    std::vector<float> y(test_dataset.size());

    for (unsigned int i = 0; i < test_dataset.size(); i++)
    {
        std::vector<float> point = data_points[i];
        y[i] = point.back();
        point.pop_back();
        X[i] = point;
    }

    float test_loss = compute_mse(y, predict(X));
    return std::make_tuple(test_dataset.size(), test_loss, test_loss);
}
