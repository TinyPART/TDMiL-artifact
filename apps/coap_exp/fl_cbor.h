/*
 * Copyright (c) 2023 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef FL_CBOR_H
#define FL_CBOR_H

#include <stdint.h>

#include "nanocbor/nanocbor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FL_CBOR_METADATA_KEY    "metadata"

#define FL_CBOR_METADATA_KEY_ROUND              "round"
#define FL_CBOR_METADATA_KEY_TOTAL_ROUND        "total_round"
#define FL_CBOR_METADATA_KEY_LOCAL_TOTAL_EPOCHS "local_total_epochs"

#define FL_CBOR_METADATA_KEY_NUM_EXAMPLES       "num_examples"
#define FL_CBOR_METADATA_KEY_VALIDATION_LOSS    "validation_loss"
#define FL_CBOR_METADATA_KEY_VALIDATION_ACCURACY    "validation_accuracy"
#define FL_CBOR_METADATA_KEY_COMPLETED_EPOCHS       "completed_epochs"

/**
 * @brief Global metadata send with the model itself
 */
typedef struct {
    uint32_t round;             /**< Round number for this model */
    uint32_t total_round;       /**< Total number of rounds */
    uint32_t local_total_epochs;    /**< Local total epochs */
} fl_global_model_metadata_t;

/**
 * @brief Local model metadata send with the model
 */
typedef struct {
    uint32_t round;             /**< Round number of the model  */
    uint32_t num_examples;      /**< Number of examples         */
    float validation_loss;      /**< Validation loss of the local model */
    float validation_accuracy;  /**< Validation accuracy of the local model */
    uint32_t completed_epochs;  /**< Number of epochs completed */
} fl_local_model_metadata_t;

/**
 * @brief Encode a global model's metadata as cbor structure
 *
 * @param enc       NanoCBOR encoder context
 * @param global    Global metadata reference to encode
 */
void fl_cbor_encode_global_model_metadata(nanocbor_encoder_t *enc, const fl_global_model_metadata_t *global);
int fl_cbor_decode_global_model_metadata(nanocbor_value_t *val, fl_global_model_metadata_t *global);

/**
 * @brief Encode a local model's metadata as cbor structure
 *
 * @param enc       NanoCBOR encoder context
 * @param global    local metadata reference to encode
 */
void fl_cbor_encode_local_model_metadata(nanocbor_encoder_t *enc, const fl_local_model_metadata_t *local);

#ifdef __cplusplus
}
#endif
#endif /* FL_CBOR_H */
