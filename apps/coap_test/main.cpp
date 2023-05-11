#include <stdio.h>
#include <vector>
#include "synthetic_dataset.h"
#include "line_fit_model.h"
#include <tuple>

#include "shell.h"
#include "msg.h"
#include "mutex.h"

#include "coap_ml.h"

#define MAIN_QUEUE_SIZE (8U)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static float _ml_values[CONFIG_COAP_ML_NUM_PARAMS] = {0};
static mutex_t values_lock = MUTEX_INIT;

static void _write_ml_values(void *ctx, float values[CONFIG_COAP_ML_NUM_PARAMS])
{
    (void)ctx; /* not used here */
    puts("Updating stored ML values");
    mutex_lock(&values_lock);
    memcpy(_ml_values, values, sizeof(_ml_values));
    mutex_unlock(&values_lock);
}

static void _read_ml_values(void *ctx, float values[CONFIG_COAP_ML_NUM_PARAMS])
{
    (void)ctx; /* not used here */
    puts("Returning stored ML values");
    mutex_lock(&values_lock);
    memcpy(values, _ml_values, sizeof(_ml_values));
    mutex_unlock(&values_lock);
}

int main()
{
    /* we need a message queue for the thread running the shell in order to
     * receive potentially fast incoming networking packets */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    coap_ml_server_init(_read_ml_values, _write_ml_values, NULL);

    std::vector<float> ms{3.5, 9.3, 5};
    float b = 1.7;
    puts("Training set:\n");
    SyntheticDataset local_training_data = SyntheticDataset(ms, b, 1000);
    SyntheticDataset local_testing_data = SyntheticDataset(ms, b, 100);
    LineFitModel local_model = LineFitModel(1000, 0.001, 3);
    std::tuple<size_t, float, float, float, float> output_train = local_model.train_SGD(local_training_data);
    printf("Training param[0]: %f\n", std::get<1>(output_train));
    printf("Training param[1]: %f\n", std::get<2>(output_train));
    printf("Training param[2]: %f\n", std::get<3>(output_train));
    printf("Training param[3]: %f\n", std::get<4>(output_train));

    std::tuple<size_t, float, float> output_test = local_model.evaluate(local_testing_data);
    printf("testing Loss: %f\n", std::get<1>(output_test));
    printf("testing Accuracy: %f\n", std::get<2>(output_test));

    puts("Starting the shell now...");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
