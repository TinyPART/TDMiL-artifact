/*
 * Copyright (C) 2023 Freie Universität Berlin
 * Copyright (C) 2023 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    coap_channel CoAP bidirectional channel for communication
 * @ingroup     sys
 * @brief       Provides a set of endpoints for bidirectional communication
 *
 * @{
 *
 * @file
 *
 * @author      Koen Zandberg <koen@bergzand.net>
 */

#ifndef COAP_CHANNEL_H
#define COAP_CHANNEL_H

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

#include "net/gcoap.h"
#include "net/nanocoap.h"
#include "net/coap.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct coap_channel coap_channel_t;

typedef struct {
    const uint8_t *payload;
    size_t payload_len;
    uint8_t cache_key[32];
    size_t offset;
    size_t num;
} coap_channel_memo_t;

typedef int (*coap_channel_callback_t)(coap_channel_t *channel,  void *payload, size_t payload_len, const coap_channel_memo_t *memo);

struct coap_channel {
    coap_channel_callback_t callback;
    gcoap_listener_t listener;
    coap_resource_t endpoint;
};

void coap_channel_init(coap_channel_t *channel, const char *path, coap_channel_callback_t callback);

/**
 * @brief Sends a CoAP payload through the specified channel.
 *
 * @param channel Pointer to the CoAP channel structure.
 * @param payload Pointer to the payload data to be sent.
 * @param payload_len Length of the payload data.
 * @return Integer indicating the status of the operation:
 *         - Returns 0 on success.
 *         - Returns a non-zero value to indicate an error.
 */
int coap_channel_send_payload(coap_channel_t *channel, uint8_t *payload, size_t payload_len);

/**
 * @brief Submits a payload through the specified CoAP channel.
 *
 * @param channel Pointer to the CoAP channel structure.
 * @param buf Pointer to the buffer containing the payload to submit.
 * @param buf_len Length of the buffer containing the payload.
 * @return Integer indicating the status of the operation:
 *         - Returns 0 on success.
 *         - Returns a non-zero value to indicate an error.
 */
int coap_channel_submit_payload(coap_channel_t *channel, void *buf, size_t buf_len);

/**
 * @brief Initializes a CoAP channel with the specified parameters.
 *
 * @param channel Pointer to the CoAP channel structure to be initialized.
 * @param path The path associated with the channel.
 * @param callback Callback function to handle channel events.
 *                 The function signature must match 'coap_channel_callback_t'.
 */
void coap_channel_init(coap_channel_t *channel, const char *path, coap_channel_callback_t callback);

#ifdef __cplusplus
}
#endif
#endif /* COAP_CHANNEL_H */
/** @} */
