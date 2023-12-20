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

#include "coap_channel.h"
#include "net/gcoap.h"
#include "net/nanocoap.h"
#include "net/coap.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_MLCONTROL_PATH       "/ml"

void mlcontrol_init(coap_channel_t *channel);

#ifdef __cplusplus
}
#endif
#endif /* ML_CONTROL_H */
/** @} */
