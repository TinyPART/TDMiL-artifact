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
#include "coap_ml.h"
#include "net/utils.h"
#include "net/gcoap.h"
#include "net/coap.h"
#include "nanocbor/nanocbor.h"

static coap_ml_read_l_params_t _read_callback = NULL;
static coap_ml_write_l_params_t _write_callback = NULL;
static void *_write_ctx = NULL;
static void *_read_ctx = NULL;

void coap_ml_set_read_l_params_callback(coap_ml_read_l_params_t func, void *ctx)
{
    _read_callback = func;
    _read_ctx = ctx;
}

void coap_ml_set_write_l_params_callback(coap_ml_write_l_params_t func, void *ctx)
{
    _write_callback = func;
    _write_ctx = ctx;
}

static ssize_t _get_l_param_c_1_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len,
                                        coap_request_ctx_t *ctx)
{
    (void)ctx;
    gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
    coap_opt_add_format(pdu, COAP_FORMAT_CBOR);
    size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

    float values[CONFIG_COAP_ML_NUM_PARAMS];
    if (_read_callback) {
        _read_callback(_read_ctx, values);
    }
    else {
        return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }

    size_t payload_remaining = len - resp_len;

    nanocbor_encoder_t enc;
    nanocbor_encoder_init(&enc, pdu->payload, payload_remaining);

    nanocbor_fmt_array(&enc, CONFIG_COAP_ML_NUM_PARAMS);
    for (size_t i = 0; i < CONFIG_COAP_ML_NUM_PARAMS; i++) {
        nanocbor_fmt_float(&enc, values[i]);
    }

    size_t encoded_len = nanocbor_encoded_len(&enc);

    if (encoded_len > payload_remaining) {
        return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }
    return resp_len + encoded_len;
}

static ssize_t _put_l_param_c_1_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len,
                                        coap_request_ctx_t *ctx)
{
    (void)ctx;
    nanocbor_value_t val, array;
    nanocbor_decoder_init(&val, pdu->payload, pdu->payload_len);
    float values[CONFIG_COAP_ML_NUM_PARAMS];
    if (!_write_callback) {
        return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }

    if (nanocbor_enter_array(&val, &array) < 0) {
        return gcoap_response(pdu, buf, len, COAP_CODE_BAD_REQUEST);
    }

    for (size_t i = 0; i < CONFIG_COAP_ML_NUM_PARAMS; i++) {
        if (nanocbor_get_float(&array, &values[i]) < 0) {
            return gcoap_response(pdu, buf, len, COAP_CODE_BAD_REQUEST);
        }
    }
    nanocbor_leave_container(&val, &array);

    if (!nanocbor_at_end(&val)) {
        return gcoap_response(pdu, buf, len, COAP_CODE_BAD_REQUEST);
    }

    _write_callback(_write_ctx, values);

    return gcoap_response(pdu, buf, len, COAP_CODE_CHANGED);
}

static ssize_t _l_param_c_1_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len,
                                    coap_request_ctx_t *ctx)
{
    /* read coap method type in packet */
    unsigned method_flag = coap_method2flag(coap_get_code_detail(pdu));

    switch (method_flag) {
    case COAP_GET:
        return _get_l_param_c_1_handler(pdu, buf, len, ctx);
    case COAP_PUT:
        return _put_l_param_c_1_handler(pdu, buf, len, ctx);
    default:
        return gcoap_response(pdu, buf, len, COAP_CODE_METHOD_NOT_ALLOWED);
    }
}

static const coap_resource_t _resources[] = {
    { "/l_param_c_1", COAP_GET | COAP_PUT, _l_param_c_1_handler, NULL },
};

static gcoap_listener_t _listener = {
    &_resources[0],
    ARRAY_SIZE(_resources),
    GCOAP_SOCKET_TYPE_UNDEF,
    gcoap_encode_link,
    NULL,
    NULL
};

void coap_ml_server_init(coap_ml_read_l_params_t read_func, coap_ml_write_l_params_t write_func,
                         void *ctx)
{
    coap_ml_set_read_l_params_callback(read_func, ctx);
    coap_ml_set_write_l_params_callback(write_func, ctx);
    gcoap_register_listener(&_listener);
}
