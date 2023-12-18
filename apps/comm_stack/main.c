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
 * @brief       communication stack example
 *
 * @author      Koen Zandberg <koen@bergzand.net>
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

#include "fmt.h"

#include "coap_channel.h"

static const char endpoint[] = "/test";

static char _stack[4096];

#define MAIN_QUEUE_SIZE (4)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

static int _callback(coap_channel_t *channel, void *payload, size_t payload_len, const coap_channel_memo_t *memo)
{
    (void)channel;
    (void)payload;
    (void)payload_len;
    (void)memo;
    return 0;
}

static void *_comm_thread(void *arg)
{
    coap_channel_t *channel = (coap_channel_t*)arg;
    unsigned req_count = 0;
    while (1) {
        char payload[5];
        size_t payload_len = fmt_u16_dec(payload, req_count);
        req_count++;
        coap_channel_send_payload(channel, (uint8_t*)payload, payload_len);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void)
{
    /* for the thread running the shell */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("comm stack example");

    ztimer_sleep(ZTIMER_MSEC, 1000);
    coap_channel_t channel;

    coap_channel_init(&channel, endpoint, _callback);

    puts("starting thread");
    thread_create(_stack, sizeof(_stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, _comm_thread, &channel, "comm");

    /* start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should never be reached */
    return 0;
}
