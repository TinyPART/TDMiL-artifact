/*
 * Copyright (C) 2023 Freie Universität Berlin
 * Copyright (C) 2023 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    coap_ml CoAP endpoints for machine learning
 * @ingroup     sys
 * @brief       Provides a set of endpoints for interacting with machine learning primitives
 *
 * @{
 *
 * @file
 * @brief       Interface for interacting with the CoAP endpoints for machine learning primitives
 *
 * @author      Koen Zandberg <koen@bergzand.net>
 */

#ifndef COAP_ML_H
#define COAP_ML_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CONFIG_COAP_ML_NUM_PARAMS
#define CONFIG_COAP_ML_NUM_PARAMS       (4)
#endif

/**
 * @brief Function prototype called to write new params received over CoAP
 *
 * @param ctx       Extra context supplied
 * @param values    Values received over CoAP that should be saved
 */
typedef void (*coap_ml_write_l_params_t)(void *ctx, float values[CONFIG_COAP_ML_NUM_PARAMS]);

/**
 * @brief Function prototype called to read stored params
 *
 * @param ctx       Extra context supplied
 * @param values    Pointer to where the stored values should be written to
 */
typedef void (*coap_ml_read_l_params_t)(void *ctx, float values[CONFIG_COAP_ML_NUM_PARAMS]);

/**
 * @brief set the callback that is called by the coap handler when reading the params
 */
void coap_ml_set_read_l_params_callback(coap_ml_read_l_params_t func, void *ctx);

/**
 * @brief set the callback that is called by the coap handler for writing new params
 */
void coap_ml_set_write_l_params_callback(coap_ml_write_l_params_t func, void *ctx);

/**
 * @brief Initialize the CoAP machine learning endpoints
 *
 * @param read_func     Function called when the GET is requested on the endpoint
 * @param write_func    Function called when a PUT is requested on the endpoint
 * @param ctx           Extra user supplied context pointer, supplied with both calls to the user
 */
void coap_ml_server_init(coap_ml_read_l_params_t read_func, coap_ml_write_l_params_t write_func,
                         void *ctx);

#ifdef __cplusplus
}
#endif
#endif /* COAP_ML_H */
/** @} */
