import asyncio

import aiocoap


class CoapChannel(object):

    def __init__(self, remote, endpoint, context, receive_callback=None):
        self.remote = remote
        self.endpoint = endpoint
        self.context = context
        self.observe_req = None
        self.receive_callback = receive_callback

    async def submit(self, payload):
        network_base = self.remote.uri
        uri = network_base + self.endpoint
        request = aiocoap.Message(uri=uri, mtype=aiocoap.CON, code=aiocoap.Code.POST, payload=bytes(payload, "UTF-8"))
        response = await self.context.request(request).response
        return response

    def _observe_result(self, *args):
        print(f"error observe: {args}")

    def _observe_err(self, *args):
        print(f"error observe: {args}")

    async def start_observe(self):
        network_base = self.remote.uri
        uri = network_base + self.endpoint
        observe_message = aiocoap.Message(code=aiocoap.Code.GET, uri=uri)
        observe_message.opt.observe = 0
        self.observe_req = self.context.request(observe_message)
        await self.observe_req.response
        self.observe_req.observation.register_errback(self._observe_err)
        self.observe_req.observation.register_callback(self._observe_result)
