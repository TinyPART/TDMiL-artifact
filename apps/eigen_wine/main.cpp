#include <stdio.h>
#include "mlmci.h"
#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include "nanocbor/nanocbor.h"
#include "blob/data/train_iid_wine_norm_0.csv.h"
#include "blob/data/train_niid_wine_norm_1.csv.h"
#include "blob/data/val_wine_norm.csv.h"
#include "blob/data/test_wine_norm.csv.h"
#include "helper.h"
#include <random>
#include <cmath>
using namespace Eigen;
using namespace std;

class NeuralNetwork
{
public:
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize)
        : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize)
    {
        // Initialize weights and biases
        W1 = MatrixXf::Random(hiddenSize, inputSize);
        b1 = VectorXf::Zero(hiddenSize);
        W2 = MatrixXf::Random(outputSize, hiddenSize);
        b2 = VectorXf::Zero(outputSize);
    }

    VectorXf relu(const VectorXf &z)
    {
        return z.array().max(0.0f);
    }

    VectorXf softmax(const VectorXf &z)
    {
        VectorXf expZ = z.array().exp();
        return expZ / expZ.sum();
    }

    VectorXf relu_derivative(const VectorXf &z)
    {
        return (z.array() > 0.0f).cast<float>();
    }

    VectorXf forward(const VectorXf &x)
    {
        hiddenLayer = relu(W1 * x + b1);
        outputLayer = softmax(W2 * hiddenLayer + b2);
        return outputLayer;
    }

    void train(const MatrixXf &train_X, const MatrixXf &train_y, const MatrixXf &val_X, const MatrixXf &val_y,
               float learningRate, int epochs, int batchSize, bool useDP = false, float epsilon = 1.0f, float delta = 1e-5f, float clipNorm = 1.0f)
    {
        std::default_random_engine generator;
        int numBatches = train_X.rows() / batchSize;
        float noise_multiplier = useDP ? computeNoiseMultiplier(epsilon, delta, batchSize, train_X.rows(), epochs) : 0.0f;

        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            // Shuffle the training data
            PermutationMatrix<Dynamic, Dynamic> perm(train_X.rows());
            perm.setIdentity();
            std::random_shuffle(perm.indices().data(), perm.indices().data() + perm.indices().size());
            MatrixXf shuffled_X = perm * train_X;
            MatrixXf shuffled_y = perm * train_y;

            for (int batch = 0; batch < numBatches; ++batch)
            {
                MatrixXf gradW1 = MatrixXf::Zero(W1.rows(), W1.cols());
                MatrixXf gradW2 = MatrixXf::Zero(W2.rows(), W2.cols());
                VectorXf gradb1 = VectorXf::Zero(b1.size());
                VectorXf gradb2 = VectorXf::Zero(b2.size());

                for (int i = batch * batchSize; i < (batch + 1) * batchSize && i < train_X.rows(); ++i)
                {
                    VectorXf x = shuffled_X.row(i);
                    VectorXf target = shuffled_y.row(i);
                    VectorXf output = forward(x);

                    // Backpropagation
                    VectorXf dL_dz2 = output - target;
                    VectorXf dL_dh = W2.transpose() * dL_dz2;
                    VectorXf dL_dz1 = dL_dh.array() * relu_derivative(hiddenLayer).array();

                    MatrixXf dW2 = dL_dz2 * hiddenLayer.transpose();
                    MatrixXf dW1 = dL_dz1 * x.transpose();

                    // Clip gradients for DP-SGD
                    if (useDP)
                    {
                        float grad_norm = sqrt(dW1.squaredNorm() + dW2.squaredNorm() + dL_dz1.squaredNorm() + dL_dz2.squaredNorm());
                        float clip_factor = std::min(1.0f, clipNorm / grad_norm);
                        dW1 *= clip_factor;
                        dW2 *= clip_factor;
                        dL_dz1 *= clip_factor;
                        dL_dz2 *= clip_factor;
                    }

                    gradW2 += dW2;
                    gradW1 += dW1;
                    gradb1 += dL_dz1;
                    gradb2 += dL_dz2;
                }

                // Add noise for DP-SGD
                if (useDP)
                {
                    std::normal_distribution<float> distribution(0.0f, clipNorm * noise_multiplier);
                    gradW1 += MatrixXf::NullaryExpr(gradW1.rows(), gradW1.cols(), [&]()
                                                    { return distribution(generator); });
                    gradW2 += MatrixXf::NullaryExpr(gradW2.rows(), gradW2.cols(), [&]()
                                                    { return distribution(generator); });
                    gradb1 += VectorXf::NullaryExpr(gradb1.size(), [&]()
                                                    { return distribution(generator); });
                    gradb2 += VectorXf::NullaryExpr(gradb2.size(), [&]()
                                                    { return distribution(generator); });
                }

                // Update weights and biases
                W2 -= learningRate * gradW2 / batchSize;
                W1 -= learningRate * gradW1 / batchSize;
                b2 -= learningRate * gradb2 / batchSize;
                b1 -= learningRate * gradb1 / batchSize;
            }

            // Evaluate the accuracy at the end of each epoch and print it
            float trainAccuracy = evaluateAccuracy(train_X, train_y);
            float valAccuracy = evaluateAccuracy(val_X, val_y);
            cout << "Epoch " << epoch + 1 << ": Train Accuracy = " << trainAccuracy * 100
                 << "%, Validation Accuracy = " << valAccuracy * 100 << "%" << endl;
        }
    }

    int predict(const VectorXf &x)
    {
        VectorXf probabilities = forward(x);
        return std::distance(probabilities.data(), std::max_element(probabilities.data(), probabilities.data() + probabilities.size()));
    }

    float evaluateAccuracy(const MatrixXf &X, const MatrixXf &y)
    {
        int correct = 0;
        for (int i = 0; i < X.rows(); ++i)
        {
            int real_label = -1;
            for (int j = 0; j < y.cols(); ++j)
            {
                if (y(i, j) == 1.0)
                {
                    real_label = j;
                    break;
                }
            }
            int predicted_label = predict(X.row(i));
            if (real_label == predicted_label)
            {
                ++correct;
            }
        }
        return static_cast<float>(correct) / X.rows();
    }

    // Getter and setter functions remain the same

private:
    int inputSize;
    int hiddenSize;
    int outputSize;
    MatrixXf W1, W2;
    VectorXf b1, b2;
    VectorXf hiddenLayer, outputLayer;

    float computeNoiseMultiplier(float epsilon, float delta, int batchSize, int datasetSize, int epochs)
    {
        float q = static_cast<float>(batchSize) / datasetSize;
        float T = epochs * (datasetSize / batchSize);
        return sqrt(2 * log(1.25 / delta)) / (epsilon / sqrt(T * q));
    }
};

int main()
{
    // Load train data
    constexpr size_t wine_aug_train_csv_size = sizeof(train_iid_wine_norm_0_csv);
    std::istringstream iss_train(std::string(reinterpret_cast<const char *>(train_iid_wine_norm_0_csv), wine_aug_train_csv_size));
    int numFeatures = 13; // Number of features
    int numLabels = 3;    // Number of label classes

    pair<MatrixXf, MatrixXf> data_train = dataLoader(iss_train, numFeatures, numLabels);
    MatrixXf X_train = data_train.first;
    MatrixXf y_train = data_train.second;

    // Load test data
    constexpr size_t wine_aug_test_csv_size = sizeof(test_wine_norm_csv);
    std::istringstream iss_test(std::string(reinterpret_cast<const char *>(test_wine_norm_csv), wine_aug_test_csv_size));
    pair<MatrixXf, MatrixXf> data_test = dataLoader(iss_test, numFeatures, numLabels);
    MatrixXf X_test = data_test.first;
    MatrixXf y_test = data_test.second;

    // Load val data
    constexpr size_t wine_aug_val_csv_size = sizeof(val_wine_norm_csv);
    std::istringstream iss_val(std::string(reinterpret_cast<const char *>(val_wine_norm_csv), wine_aug_val_csv_size));
    pair<MatrixXf, MatrixXf> data_val = dataLoader(iss_val, numFeatures, numLabels);
    MatrixXf X_val = data_val.first;
    MatrixXf y_val = data_val.second;

    // Create neural networks
    int inputSize = X_train.cols();
    int hiddenSize = 8;
    int outputSize = y_train.cols();
    NeuralNetwork nn_without_dp(inputSize, hiddenSize, outputSize);
    NeuralNetwork nn_with_dp(inputSize, hiddenSize, outputSize);

    // Training parameters
    float learningRate = 0.009;
    int epochs = 200;
    int batchSize = 64;
    float epsilon = 1.0;
    float delta = 1e-5;
    float clipNorm = 1.0;

    // Train without DP
    cout << "Training without Differential Privacy:" << endl;
    nn_without_dp.train(X_train, y_train, X_val, y_val, learningRate, epochs, batchSize);

    // Train with DP
    cout << "\nTraining with Differential Privacy:" << endl;
    nn_with_dp.train(X_train, y_train, X_val, y_val, learningRate, epochs, batchSize, true, epsilon, delta, clipNorm);

    // Evaluate and compare results
    float accuracy_without_dp = nn_without_dp.evaluateAccuracy(X_test, y_test);
    float accuracy_with_dp = nn_with_dp.evaluateAccuracy(X_test, y_test);

    cout << "\nComparison Results:" << endl;
    cout << "Test Accuracy without DP: " << accuracy_without_dp * 100 << "%" << endl;
    cout << "Test Accuracy with DP: " << accuracy_with_dp * 100 << "%" << endl;
    cout << "Accuracy difference: " << (accuracy_without_dp - accuracy_with_dp) * 100 << " percentage points" << endl;

    return 0;
}