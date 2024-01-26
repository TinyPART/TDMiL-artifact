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
#include "coap_channel.h"
#include "net/gcoap.h"
#include "net/coap.h"

#define ENABLE_DEBUG 0
#include "debug.h"

int coap_channel_send_payload(coap_channel_t *channel, uint8_t *payload, size_t payload_len)
{
    (void)payload;
    (void)payload_len;
    size_t len;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    switch (gcoap_obs_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE,
            &channel->endpoint)) {
    case GCOAP_OBS_INIT_OK:
        DEBUG("coap_channel: creating notification\n");
        coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
        coap_opt_add_format(&pdu, COAP_FORMAT_CBOR);
        len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
        gcoap_obs_send(&buf[0], len, &channel->endpoint);
        break;
    case GCOAP_OBS_INIT_UNUSED:
        DEBUG("coap_channel: no observer on channel\n");
        break;
    case GCOAP_OBS_INIT_ERR:
        DEBUG("coap_channel: error initializing notification\n");
        break;
    return 0;
    }
    return 0;
}

static ssize_t _endpoint_handle_post(coap_pkt_t *pkt, uint8_t *buf, size_t len, coap_channel_t *channel, coap_request_ctx_t *context)
{
    uint8_t req_buf[128];
    coap_channel_memo_t memo;
    nanocoap_cache_key_generate(pkt, memo.cache_key);

    memo.payload_len = MIN(sizeof(req_buf), pkt->payload_len);
    memo.payload = req_buf;
    memo.ctx = channel->ctx;
    memo.remote = context->remote;

    memcpy(req_buf, pkt->payload, memo.payload_len);
    gcoap_resp_init(pkt, buf, len, COAP_CODE_CHANGED);
    coap_opt_add_format(pkt, COAP_FORMAT_CBOR);
    ssize_t resp_len = coap_opt_finish(pkt, COAP_OPT_FINISH_PAYLOAD);

    ssize_t res = channel->callback(channel, pkt->payload, pkt->payload_len, &memo);
    if (res < 0) {
        return gcoap_response(pkt, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }
    else if (res == 0) {
        return gcoap_response(pkt, buf, len, COAP_CODE_CHANGED);
    }

    return resp_len + res;
}

static ssize_t _endpoint_handler(coap_pkt_t *pkt, uint8_t *buf, size_t len, coap_request_ctx_t *context)
{
    coap_channel_t *channel = coap_request_ctx_get_context(context);
    (void)buf;
    (void)len;

    /* read coap method type in packet */
    unsigned method_flag = coap_method2flag(coap_get_code_detail(pkt));

    switch (method_flag) {
        case COAP_GET:
            gcoap_resp_init(pkt, buf, len, COAP_CODE_CONTENT);
            size_t resp_len = coap_opt_finish(pkt, COAP_OPT_FINISH_NONE);
            return resp_len;
        case COAP_POST:
        {
            return _endpoint_handle_post(pkt, buf, len, channel, context);
        }
    }
    return 0;
}

void coap_channel_init(coap_channel_t *channel, const char *path, coap_channel_callback_t callback, void *ctx)
{
    memset(channel, 0, sizeof(coap_channel_t));

    channel->callback = callback;
    channel->ctx = ctx;

    channel->endpoint.path = path;
    channel->endpoint.methods = COAP_GET | COAP_POST;
    channel->endpoint.handler = _endpoint_handler;
    channel->endpoint.context = channel;
    channel->listener.tl_type = GCOAP_SOCKET_TYPE_UNDEF;
    channel->listener.resources = &channel->endpoint;
    channel->listener.resources_len = 1;

    gcoap_register_listener(&channel->listener);
}
