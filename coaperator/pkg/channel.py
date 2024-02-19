"""Async communication channel from/to CoAP clients."""
import aiocoap
import aiocoap.error
import aiocoap.numbers.contentformat
import logging
from typing import Optional


class CoapChannel(object):
    """CoAP channel based on observe/post CoAP requests.

    Establishes a bidirectional channel with a CoAP client
    """

    def __init__(
        self,
        remote,
        endpoint,
        context,
        receive_callback=None,
        logger: Optional[logging.Logger] = None,
    ):
        self.remote = remote
        self.endpoint = endpoint
        self.context = context
        self.observe_req = None
        self.receive_callback = receive_callback
        if logger:
            self.logger = logger.getChild("channel")
        else:
            self.logger = logging.getLogger("device.channel")

    async def submit(self, payload: bytes) -> Optional[aiocoap.Message]:
        """Submit a message to the connected client.

        :param payload: Payload to send to the client
        :return: Response CoAP message
        """
        network_base = self.remote.uri
        uri = network_base + self.endpoint
        request = aiocoap.Message(
            uri=uri, mtype=aiocoap.CON, code=aiocoap.Code.POST, payload=payload,
            content_format=aiocoap.numbers.contentformat.ContentFormat.CBOR
        )
        try:
            response = await self.context.request(request).response
        except aiocoap.error.Error as e:
            self.logger.error(f"Unable to submit data over channel: {e}")
            return None
        return response

    def _observe_result(self, *args):
        self.logger.info(f"error observe: {args}")

    def _observe_err(self, *args):
        self.logger.error(f"error observe: {args}")

    async def start_observe(self, channel_cb, err_cb):
        network_base = self.remote.uri
        uri = network_base + self.endpoint
        observe_message = aiocoap.Message(code=aiocoap.Code.GET, uri=uri)
        observe_message.opt.observe = 0
        self.observe_req = self.context.request(observe_message)
        try:
            await self.observe_req.response
        except aiocoap.error.Error as e:
            self.logger.error(f"Unable to establish observe channel: {e}")
        self.observe_req.observation.register_errback(channel_cb)
        self.observe_req.observation.register_callback(err_cb)
