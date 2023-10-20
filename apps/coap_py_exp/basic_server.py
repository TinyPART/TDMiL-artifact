#!/usr/bin/env python3

# SPDX-FileCopyrightText: Christian Amsüss and the aiocoap contributors
#
# SPDX-License-Identifier: MIT

"""This is a usage example of aiocoap that demonstrates how to implement a
simple server. See the "Usage Examples" section in the aiocoap documentation
for some more information."""

AIOCOAP_DTLSSERVER_ENABLED = 1
import datetime
import logging

import asyncio

import aiocoap.resource as resource
from aiocoap.numbers.contentformat import ContentFormat
import aiocoap


class BlockResource(resource.Resource):
    """Example resource which supports the GET and PUT methods. It sends large
    responses, which trigger blockwise transfer."""

    def __init__(self):
        super().__init__()
        self.set_content(
            b"This is the resource's default content. It is padded "
            b"with numbers to be large enough to trigger blockwise "
            b"transfer.\n"
        )

    def set_content(self, content):
        self.content = content
        # while len(self.content) <= 1024:
        #     self.content = self.content + b"0123456789\n"

    async def render_get(self, request):
        return aiocoap.Message(payload=self.content)

    async def render_put(self, request):
        print("PUT payload: %s" % request.payload)
        self.set_content(request.payload)
        return aiocoap.Message(code=aiocoap.CHANGED, payload=self.content)


# logging setup

logging.basicConfig(level=logging.INFO)
logging.getLogger("coap-server").setLevel(logging.DEBUG)


async def main():
    # Resource tree creation
    root = resource.Site()

    root.add_resource(["other"], BlockResource())

    await aiocoap.Context.create_server_context(root)
    # server_context = await aiocoap.Context.create_server_context(root, bind=('::1', 5683))
    # server_context.server_credentials.load_from_dict(
    #     {
    #         ":client1": {
    #             "dtls": {
    #                 "psk": b"secretPSK",
    #                 "client-identity": b"client_Identity",
    #             }
    #         }
    #     }
    # )

    # Run forever
    await asyncio.get_running_loop().create_future()


if __name__ == "__main__":
    asyncio.run(main())
