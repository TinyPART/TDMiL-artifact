/*
 * Copyright (c) 2023 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fl_coap_client.h"
#include "fl_cbor.h"
#include "fmt.h"
#include "net/coap.h"
#include "net/nanocoap.h"
#include "net/gcoap.h"
#include "net/nanocoap/nanocbor_helper.h"
#include "macros/xtstr.h"
#include "net/sock/util.h"
#include "net/utils.h"

#define ENABLE_DEBUG 1
#include "debug.h"

static const char uri_global_model[] = "/global_model";
static const char uri_local_model[] = "/local_model/c_" XTSTR(FL_CLIENT_ID);

ssize_t _send_local_model_pdu(const sock_udp_ep_t *remote, const fl_local_model_metadata_t *metadata, fl_coap_client_ctx_t *ctx);

static void _local_model_resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t* pdu,
                          const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    fl_coap_client_ctx_t *ctx = memo->context;
    switch (memo->state) {
        case GCOAP_MEMO_TIMEOUT:
        case GCOAP_MEMO_ERR:
        case GCOAP_MEMO_RESP_TRUNC:
            ctx->state = FL_COAP_CLIENT_FAILED;
            DEBUG("[fl_client]: Error transmitting request\n");
            break;
        case GCOAP_MEMO_RESP:
            if (coap_get_code_raw(pdu) == COAP_CODE_CONTINUE) {
                _send_local_model_pdu(remote, ctx->metadata, ctx);
            }
            else {
                DEBUG("[fl_client]: Received %u from server\n", coap_get_code(pdu));
            }
    }
}

static void _local_model_obs_resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t* pdu,
                          const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    fl_coap_client_ctx_t *ctx = memo->context;
    switch (memo->state) {
        case GCOAP_MEMO_TIMEOUT:
        case GCOAP_MEMO_ERR:
        case GCOAP_MEMO_RESP_TRUNC:
            ctx->state = FL_COAP_CLIENT_FAILED;
            DEBUG("[fl_client]: Error transmitting request\n");
            break;
        case GCOAP_MEMO_RESP:
            DEBUG("[fl_client]: Received %u from server\n", coap_get_code(pdu));
    }
}

ssize_t _send_local_model_pdu(const sock_udp_ep_t  *remote, const fl_local_model_metadata_t *metadata, fl_coap_client_ctx_t *ctx)
{
    uint8_t buf[FL_CLIENT_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    coap_block_slicer_t slicer;
    coap_block_slicer_init(&slicer, ctx->block_num, 64);

    gcoap_req_init(&pdu, buf, FL_CLIENT_PDU_BUF_SIZE, COAP_METHOD_PUT, NULL);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_opt_add_etag_dummy(&pdu, COAP_ETAG_LENGTH_MAX);
    coap_opt_add_uri_path(&pdu, uri_local_model);
    coap_opt_add_format(&pdu, COAP_FORMAT_CBOR);
    coap_opt_add_block1(&pdu, &slicer, true);
    ssize_t plen = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    assert(plen > 0);

    nanocbor_encoder_t encoder;
    coap_nanocbor_slicer_helper_t helper;
    coap_nanocbor_slicer_helper_init(&helper, &pdu, &slicer);
    coap_nanocbor_encoder_init(&encoder, &helper);

    fl_cbor_encode_local_model_metadata(&encoder, metadata);

    ssize_t res = coap_nanocbor_block1_finish(&pdu, &helper);

    return gcoap_req_send(buf, res, remote, _local_model_resp_handler, ctx);
}

ssize_t _send_global_model_observe(const sock_udp_ep_t *remote, uint32_t round)
{
    uint8_t buf[FL_CLIENT_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, FL_CLIENT_PDU_BUF_SIZE, COAP_METHOD_GET, NULL);
    /* Add observer */
    uint32_t now = ztimer_now(ZTIMER_USEC);
    pdu.observe_value = (now >> GCOAP_OBS_TICK_EXPONENT) & 0xFFFFFF;
    coap_opt_add_uint(&pdu, COAP_OPT_OBSERVE, pdu.observe_value);
    coap_opt_add_uri_path(&pdu, uri_global_model);
    coap_opt_add_format(&pdu, COAP_FORMAT_CBOR);

    char round_str[5] = { 0 };
    fmt_u32_dec(round_str, round);
    coap_opt_add_uri_query(&pdu, "round", round_str);

    ssize_t plen = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    return gcoap_req_send(buf, plen, remote, _local_model_obs_resp_handler, NULL);
}

void fl_coap_submit_local_model_async(const sock_udp_ep_t *remote,
        const fl_local_model_metadata_t *metadata,
        fl_coap_client_ctx_t *ctx)
{
    memset(ctx, 0, sizeof(fl_coap_client_ctx_t));
    ctx->state = FL_COAP_CLIENT_START;
    ctx->metadata = metadata;
    ssize_t bytes_sent = _send_local_model_pdu(remote, metadata, ctx);
    if (bytes_sent > 0) {
        printf("Sent CoAP request\n");
        ctx->block_num++;
    }
    else {
        printf("Error sending CoAP request: %d\n", bytes_sent);
    }
}

void fl_coap_start_global_model_observe(const sock_udp_ep_t *remote, uint32_t round)
{
    _send_global_model_observe(remote, round);
}
