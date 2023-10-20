/*
 * Copyright (c) 2023 Koen Zandberg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */
#include <string.h>
#include <stdint.h>
#include "fl_cbor.h"

static void _put_map_key_ints(nanocbor_encoder_t *enc, const char *str, uint32_t val)
{
    nanocbor_put_tstr(enc, str);
    nanocbor_fmt_uint(enc, val);
}

static void _put_map_key_floats(nanocbor_encoder_t *enc, const char *str, float val)
{
    nanocbor_put_tstr(enc, str);
    nanocbor_fmt_float(enc, val);
}

void fl_cbor_encode_global_model_metadata(nanocbor_encoder_t *enc, const fl_global_model_metadata_t *global)
{
    nanocbor_put_tstr(enc, FL_CBOR_METADATA_KEY);
    nanocbor_fmt_map(enc, 3);

    _put_map_key_ints(enc, FL_CBOR_METADATA_KEY_ROUND, global->round);
    _put_map_key_ints(enc, FL_CBOR_METADATA_KEY_TOTAL_ROUND, global->total_round);
    _put_map_key_ints(enc, FL_CBOR_METADATA_KEY_LOCAL_TOTAL_EPOCHS, global->local_total_epochs);
}

static int _decode_map_key_ints(nanocbor_value_t *map, const char *key, uint32_t *val)
{
    nanocbor_value_t cbor_key;
    int res = nanocbor_get_key_tstr(map, key, &cbor_key);
    if (res == NANOCBOR_OK) {
        nanocbor_skip_simple(&cbor_key);
        nanocbor_get_uint32(&cbor_key, val);
    }
    return res;
}

int fl_cbor_decode_global_model_metadata(nanocbor_value_t *val, fl_global_model_metadata_t *global)
{
    nanocbor_value_t map;
    int res = nanocbor_enter_map(val, &map);
    if (res < NANOCBOR_OK) {
        return res;
    }

    res = _decode_map_key_ints(&map, FL_CBOR_METADATA_KEY_ROUND, &global->round);
    if (res < NANOCBOR_OK) {
        return res;
    }
    res = _decode_map_key_ints(&map, FL_CBOR_METADATA_KEY_TOTAL_ROUND, &global->total_round);
    if (res < NANOCBOR_OK) {
        return res;
    }
    res = _decode_map_key_ints(&map, FL_CBOR_METADATA_KEY_LOCAL_TOTAL_EPOCHS,
            &global->local_total_epochs);
    return res;
}

void fl_cbor_encode_local_model_metadata(nanocbor_encoder_t *enc, const fl_local_model_metadata_t *local)
{
    nanocbor_put_tstr(enc, FL_CBOR_METADATA_KEY);
    nanocbor_fmt_map(enc, 5);
    _put_map_key_ints(enc, FL_CBOR_METADATA_KEY_ROUND, local->round);
    _put_map_key_ints(enc, FL_CBOR_METADATA_KEY_NUM_EXAMPLES, local->num_examples);
    _put_map_key_floats(enc, FL_CBOR_METADATA_KEY_VALIDATION_LOSS, local->validation_loss);
    _put_map_key_floats(enc, FL_CBOR_METADATA_KEY_VALIDATION_ACCURACY, local->validation_accuracy);
    _put_map_key_ints(enc, FL_CBOR_METADATA_KEY_COMPLETED_EPOCHS, local->completed_epochs);
}

