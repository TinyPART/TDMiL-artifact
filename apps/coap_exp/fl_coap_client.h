/*
 * Copyright (c) 2023 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef FL_COAP_CLIENT_H
#define FL_COAP_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fl_coap_client.h"
#include "fl_cbor.h"
#include "net/sock/util.h"
#include "net/utils.h"

#ifndef FL_CLIENT_ID
#error The FL_CLIENT_ID must be set to an integer value
#endif

#define FL_CLIENT_PDU_BUF_SIZE 127

typedef enum {
    FL_COAP_CLIENT_START = 0,
    FL_COAP_CLIENT_PROGRESS,
    FL_COAP_CLIENT_COMPLETED,
    FL_COAP_CLIENT_FAILED,
} fl_coap_client_state_t;

typedef struct {
    uint32_t block_num;
    fl_coap_client_state_t state;
    const fl_local_model_metadata_t *metadata;
} fl_coap_client_ctx_t;

/**
 * @brief Start transfer of the local model to the server
 *
 * This call starts an async transfer of the local model data to the server. The metadata and
 * context struct pointers must remain valid throughout the full CoAP
 *
 * @param   remote      Sock UDP endpoint to the remote server
 * @param   metadata    Struct with the metadata to include in the CBOR payload
 * @param   ctx         Context struct for the full transfer
 */
void fl_coap_submit_local_model_async(const sock_udp_ep_t *remote, const fl_local_model_metadata_t *metadata, fl_coap_client_ctx_t *ctx);

/**
 * @brief Start the observe subscription on the server for the global model
 *
 * @param   remote      Sock UDP endpoint to the remote server
 * @param   round       Round number to observe
 */
void fl_coap_start_global_model_observe(const sock_udp_ep_t *remote, uint32_t round);
#ifdef __cplusplus
}
#endif
#endif
