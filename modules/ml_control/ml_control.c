/*
 * Copyright (C) 2023 Freie Universität Berlin
 * Copyright (C) 2023 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ml_control.h"
#include "ml_control_numbers.h"
#include "net/gcoap.h"
#include "net/coap.h"

#include "nanocbor/nanocbor.h"

#define ENABLE_DEBUG 0
#include "debug.h"

static int _handle_rpc_status(coap_channel_t *channel, nanocbor_value_t *arr, nanocbor_encoder_t *encoder)
{
    (void)channel;
    (void)arr;
    nanocbor_fmt_array(encoder, 1);
    nanocbor_fmt_uint(encoder, 0);
    return 0;
}

static int _handle_rpc(coap_channel_t *channel, nanocbor_value_t *arr, nanocbor_encoder_t *encoder, uint32_t rpc)
{
    switch (rpc) {
        case MLCONTROL_RPC_STATUS:
            return _handle_rpc_status(channel, arr, encoder);
        case MLCONTROL_RPC_START:
            return 0;
        case MLCONTROL_RPC_STOP:
            return 0;
        default:
            return -1;
    }
}

static int _channel_callback(coap_channel_t *channel,  void *payload, size_t payload_len, const coap_channel_memo_t *memo)
{
    nanocbor_value_t decoder, arr;
    nanocbor_decoder_init(&decoder, memo->payload, memo->payload_len);

    nanocbor_encoder_t encoder;
    nanocbor_encoder_init(&encoder, payload, payload_len);

    int res = nanocbor_enter_array(&decoder, &arr);

    if (res != NANOCBOR_OK) {
        return -1;
    }

    uint32_t rpc_command = 0;
    res = nanocbor_get_uint32(&arr, &rpc_command);

    if (res < NANOCBOR_OK) {
        return -1;
    }
    res = _handle_rpc(channel, &arr, &encoder, rpc_command);
    return MIN(nanocbor_encoded_len(&encoder), payload_len);
}


void mlcontrol_init(coap_channel_t *channel)
{
    coap_channel_init(channel, CONFIG_MLCONTROL_PATH, _channel_callback);
}
