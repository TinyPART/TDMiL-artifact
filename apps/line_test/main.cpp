#include <stdio.h>
#include <vector>
#include "synthetic_dataset.h"
#include "line_fit_model.h"
#include <tuple>

int main()
{

    std::vector<float> ms{3.5, 9.3, 5}; 
    float b = 1.7;
    puts("Training set:\n");
    SyntheticDataset local_training_data = SyntheticDataset(ms, b, 1000);
    SyntheticDataset local_testing_data = SyntheticDataset(ms, b, 100);
    LineFitModel local_model = LineFitModel(1000, 0.001, 3);
    std::tuple<size_t, float, float> output_train = local_model.train_SGD(local_training_data);
    printf("Training Loss: %f\n", std::get<1>(output_train));
    printf("Training Accuracy: %f\n", std::get<2>(output_train));

    std::tuple<size_t, float, float> output_test = local_model.evaluate(local_testing_data);
    printf("testing Loss: %f\n", std::get<1>(output_test));
    printf("testing Accuracy: %f\n", std::get<2>(output_test));

    return 0;
}
