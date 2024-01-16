#include <stdio.h>
#include "mlmci.h"
#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include "nanocbor/nanocbor.h"
#include "blob/iris_data/iris_aug_train.csv.h"
#include "blob/iris_data/iris_aug_test.csv.h"
#include "blob/data/train_iid_iris_norm_0.csv.h"
#include "blob/data/train_niid_iris_norm_1.csv.h"
#include "blob/data/val_iris_norm.csv.h"
#include "blob/data/test_iris_norm.csv.h"
#include "helper.h"
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
        return z.array().max(0.0);
    }
    VectorXf softmax(const VectorXf &z)
    {
        VectorXf expZ = z.array().exp();
        return expZ / expZ.sum();
    }
    VectorXf relu_derivative(const VectorXf &z)
    {
        return (z.array() > 0.0).cast<float>();
    }
    VectorXf forward(const VectorXf &x)
    {
        hiddenLayer = relu(W1 * x + b1);
        outputLayer = softmax(W2 * hiddenLayer + b2);
        return outputLayer;
    }

    void train(const MatrixXf &train_X, const MatrixXf &train_y, const MatrixXf &test_X, const MatrixXf &test_y, float learningRate, int epochs)
    {
        for (int epoch = 0; epoch < epochs; ++epoch)
        {
            for (int i = 0; i < train_X.rows(); ++i)
            {
                // Forward pass
                VectorXf x = train_X.row(i);
                VectorXf target = train_y.row(i);
                outputLayer = forward(x);

                // Backpropagation
                VectorXf dL_dz2 = outputLayer - target;
                VectorXf dL_dh = W2.transpose() * dL_dz2;
                VectorXf dL_dz1 = dL_dh.array() * relu_derivative(hiddenLayer).array();
                ;

                // Update weights and biases
                W2 -= learningRate * dL_dz2 * hiddenLayer.transpose();
                b2 -= learningRate * dL_dz2;
                W1 -= learningRate * dL_dz1 * x.transpose();
                b1 -= learningRate * dL_dz1;
            }

            // Evaluate the accuracy at the end of each epoch and print it
            float accuracy = evaluateAccuracy(test_X, test_y);
            cout << "Epoch " << epoch + 1 << ": Accuracy = " << accuracy * 100 << endl;
        }
    }

    int predict(const VectorXf &x)
    {
        VectorXf probabilities = forward(x);
        int predictedClass = 0;
        float maxProbability = probabilities[0];

        for (int i = 1; i < probabilities.size(); ++i)
        {
            if (probabilities[i] > maxProbability)
            {
                maxProbability = probabilities[i];
                predictedClass = i;
            }
        }

        return predictedClass;
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

    // Getter functions
    MatrixXf getW1() const
    {
        return W1;
    }

    MatrixXf getW2() const
    {
        return W2;
    }

    VectorXf getb1() const
    {
        return b1;
    }

    VectorXf getb2() const
    {
        return b2;
    }

    // Setter functions
    void setW1(const MatrixXf &newW1)
    {
        W1 = newW1;
    }

    void setW2(const MatrixXf &newW2)
    {
        W2 = newW2;
    }

    void setb1(const VectorXf &newb1)
    {
        b1 = newb1;
    }

    void setb2(const VectorXf &newb2)
    {
        b2 = newb2;
    }

private:
    int inputSize;
    int hiddenSize;
    int outputSize;
    MatrixXf W1, W2;
    VectorXf b1, b2;
    VectorXf hiddenLayer, outputLayer;
};
int main()
{ // Load train data
    constexpr size_t iris_aug_train_csv_size = sizeof(train_niid_iris_norm_1_csv);

    std::istringstream iss_train(std::string(reinterpret_cast<const char *>(train_niid_iris_norm_1_csv), iris_aug_train_csv_size));
    int numFeatures = 4; // Number of features
    int numLabels = 3;   // Number of label classes

    pair<MatrixXf, MatrixXf> data_train = dataLoader(iss_train, numFeatures, numLabels);
    MatrixXf X_train = data_train.first;
    MatrixXf y_train = data_train.second;
    // Load test data
    constexpr size_t iris_aug_test_csv_size = sizeof(test_iris_norm_csv);

    std::istringstream iss_test(std::string(reinterpret_cast<const char *>(test_iris_norm_csv), iris_aug_test_csv_size));

    pair<MatrixXf, MatrixXf> data_test = dataLoader(iss_test, numFeatures, numLabels);
    MatrixXf X_test = data_test.first;
    MatrixXf y_test = data_test.second;
    // Load val data
    constexpr size_t iris_aug_val_csv_size = sizeof(val_iris_norm_csv);

    std::istringstream iss_val(std::string(reinterpret_cast<const char *>(val_iris_norm_csv), iris_aug_val_csv_size));

    pair<MatrixXf, MatrixXf> data_val = dataLoader(iss_val, numFeatures, numLabels);
    MatrixXf X_val = data_val.first;
    MatrixXf y_val = data_val.second;

    // Create and train the neural network
    int inputSize = X_train.cols();
    int hiddenSize = 2;
    int outputSize = y_train.cols();
    NeuralNetwork nn(inputSize, hiddenSize, outputSize);

    float learningRate = 0.001;
    int epochs = 200;
    puts("Started Training...");
    nn.train(X_train, y_train, X_val, y_val, learningRate, epochs);
    puts("Completed Training...");
    MatrixXf W1_trained = nn.getW1();
    VectorXf b1_trained = nn.getb1();
    MatrixXf W2_trained = nn.getW2();
    VectorXf b2_trained = nn.getb2();

    cout << "Original W1_trained:\n"
         << W1_trained << endl;
    cout << "Original b1_trained:\n"
         << b1_trained << endl;
    cout << "Original W2_trained:\n"
         << W2_trained << endl;
    cout << "Original b2_trained:\n"
         << b2_trained << endl;
    // Calculate total size
    size_t totalSize = W1_trained.size() + b1_trained.size() + W2_trained.size() + b2_trained.size();

    // Create a standard array
    float *float_array = new float[totalSize];

    // Flatten matrices and vectors into the array
    size_t index = 0;

    for (int i = 0; i < W1_trained.size(); ++i)
        float_array[index++] = W1_trained(i);
    for (int i = 0; i < b1_trained.size(); ++i)
        float_array[index++] = b1_trained(i);
    for (int i = 0; i < W2_trained.size(); ++i)
        float_array[index++] = W2_trained(i);
    for (int i = 0; i < b2_trained.size(); ++i)
        float_array[index++] = b2_trained(i);
    cout << W1_trained.size();
    // Convert the float array to a uint8_t*
    uint8_t *buffer = (uint8_t *)float_array;

    // Create and populate mlmodel_param_t
    mlmodel_param_t param;
    const char *name_value = "initial_param";
    param.name = name_value;
    param.values = buffer;
    param.num_bytes = totalSize;
    param.permission = MLMODEL_PARAM_PERMISSION_WRITE;
    param.volatile_values = nullptr;
    param.persistent_values = nullptr;
    std::cout << "param.name: " << param.name << std::endl;
    float *float_array_back = (float *)param.values;

    // Create matrices and vectors
    MatrixXf W1_back = Map<MatrixXf>(float_array_back, hiddenSize, inputSize);
    float_array_back += W1_back.size();

    VectorXf b1_back = Map<VectorXf>(float_array_back, hiddenSize);
    float_array_back += b1_back.size();

    MatrixXf W2_back = Map<MatrixXf>(float_array_back, outputSize, hiddenSize);
    float_array_back += W2_back.size();

    VectorXf b2_back = Map<VectorXf>(float_array_back, outputSize);
    // print deserialized values
    cout << "W1_back:\n"
         << W1_back << endl;
    cout << "b1_back:\n"
         << b1_back << endl;
    cout << "W2_back:\n"
         << W2_back << endl;
    cout << "b2_back:\n"
         << b2_back << endl;
    // Print deserialized values
    cout << "Shape of W1_back: " << W1_back.rows() << "x" << W1_back.cols() << endl;

    cout << "Size of b1_back: " << b1_back.size() << endl;

    cout << "Shape of W2_back: " << W2_back.rows() << "x" << W2_back.cols() << endl;

    cout << "Size of b2_back: " << b2_back.size() << endl;

    float accuracy = nn.evaluateAccuracy(X_test, y_test);
    cout << " Test Accuracy = " << accuracy * 100 << endl;

    return 0;
}
