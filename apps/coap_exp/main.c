/*
 * Copyright (c) 2015-2016 Ken Bannister. All rights reserved.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       gcoap example
 *
 * @author      Ken Bannister <kb2ma@runbox.com>
 *
 * @}
 */

#include <stdio.h>
#include "msg.h"

#include "net/gcoap.h"
#include "shell.h"
#include "net/sock.h"
#include "net/sock/util.h"
#include "net/utils.h"

#include "fl_coap_client.h"
#include "fl_cbor.h"

#define MAIN_QUEUE_SIZE (4)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

static const char addr_str[] = "[2001:db8::b0da:2dff:fe62:a4a3]";

int main(void)
{
    /* for the thread running the shell */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("gcoap example app");

    ztimer_sleep(ZTIMER_MSEC, 3000);
    sock_udp_ep_t remote;
    if (sock_udp_name2ep(&remote, addr_str) != 0) {
        printf("Failed to convert name: %s\n", addr_str);
    }
    remote.port = COAP_PORT;
    fl_coap_start_global_model_observe(&remote, 0);

    fl_local_model_metadata_t metadata = {0};
    fl_coap_client_ctx_t ctx;

    fl_coap_submit_local_model_async(&remote, &metadata, &ctx);

    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should never be reached */
    return 0;
}
