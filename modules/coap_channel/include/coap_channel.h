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

/**
 * @brief Submits a payload to subscribers
 */
int coap_channel_send_payload(coap_channel_t *channel, uint8_t *payload, size_t payload_len);
int coap_channel_submit_payload(coap_channel_t *channel, void *buf, size_t buf_len);

void coap_channel_init(coap_channel_t *channel, const char *path, coap_channel_callback_t callback);
#ifdef __cplusplus
}
#endif
#endif /* COAP_CHANNEL_H */
/** @} */
