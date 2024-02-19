/*
 * Copyright (C) 2023 Freie Universität Berlin
 * Copyright (C) 2023 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    ml_control Control data of ML models for FL
 * @ingroup     sys
 * @brief       Provides RPC state machine control
 *
 * This module is a singleton, only a single instance exists
 *
 * @{
 *
 * @file
 *
 * @author      Koen Zandberg <koen@bergzand.net>
 *
 * Generic RPC structure consists of an array with the first element being the command ID
 * Implemented
 *
 * - Get status
 * -
 *
 */

#ifndef ML_CONTROL_H
#define ML_CONTROL_H

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

#include "uuid.h"
#include "coap_channel.h"
#include "net/gcoap.h"
#include "net/nanocoap.h"
#include "net/coap.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_MLCONTROL_PATH       "/ev"
#define CONFIG_MLCONTROL_MODEL_PATH       "/ml"
#define MLCONTROL_STACK_SIZE        4096
#define MLCONTROL_FETCH_ARG_SIZE        128

typedef enum {
    MLCONTROL_ERR_INVALID_REQ_STRUCTURE = 0,
} mlcontrol_error_t;

typedef struct {
    uuid_t identifier;
    uint8_t model_layer_args[MLCONTROL_FETCH_ARG_SIZE];
    size_t args_len;
    size_t current_block;
    sock_udp_ep_t remote;
} mlcontrol_model_fetch_t;

typedef struct {
    uuid_t identifier;
    uint8_t model_layer_args[MLCONTROL_FETCH_ARG_SIZE];
    size_t args_len;
    size_t current_block;
    sock_udp_ep_t remote;
} mlcontrol_model_upload_t;

typedef struct {
    event_t fetch_ev;
    event_t upload_ev;
    coap_channel_t *channel;
    event_queue_t queue;
    mlcontrol_model_fetch_t fetch;
    mlcontrol_model_upload_t upload;
    bool training;
} mlcontrol_t;

void mlcontrol_init(mlcontrol_t *control, coap_channel_t *channel);

#ifdef __cplusplus
}
#endif
#endif /* ML_CONTROL_H */
/** @} */
