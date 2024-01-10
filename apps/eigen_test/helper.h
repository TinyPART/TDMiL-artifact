// helper.h

#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <Eigen/Dense>
using namespace std;


pair<Eigen::MatrixXf, Eigen::MatrixXf> dataLoader(istringstream &iss, int numFeatures, int numLabels);

#endif // HELPER_H