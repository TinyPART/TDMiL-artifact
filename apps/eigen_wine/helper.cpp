#include "helper.h"

pair<Eigen::MatrixXf, Eigen::MatrixXf> dataLoader(istringstream &iss, int numFeatures, int numLabels)
{

    std::string line;
    std::vector<std::vector<float>> data;

    while (std::getline(iss, line, '\n'))
    {
        std::istringstream line_stream(line);
        std::string cell;
        std::vector<float> row;

        while (std::getline(line_stream, cell, ','))
        {
            float value = std::stof(cell);
            row.push_back(value);
        }

        data.push_back(row);
    }

    int numRows = data.size();
    Eigen::MatrixXf X(numRows, numFeatures);
    Eigen::MatrixXf y(numRows, numLabels);
    y.setZero();

    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numFeatures; ++j)
        {
            X(i, j) = data[i][j];
        }

        int labelIndex = static_cast<int>(data[i].back()); // Last value is the label
        y(i, labelIndex) = 1.0;
    }

    return make_pair(X, y);
}