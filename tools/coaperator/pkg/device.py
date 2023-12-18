import asyncio
import logging

from pkg.corerd import Registration
from pkg.channel import CoapChannel

class Device(Registration):
    def __init__(self, static_registration_parameters, path, network_remote, delete_cb, update_cb,
                 registration_parameters, proxy_host, setproxyremote_cb, context=None):
        super().__init__(static_registration_parameters, path, network_remote, delete_cb, update_cb,
                         registration_parameters, proxy_host, setproxyremote_cb, context)
        name = '/'.join(static_registration_parameters['ep'])
        self.logger = logging.getLogger(f"device/{name}")
        self.logger.info(f"New device at {network_remote.sockaddr[0]}")
        self.channel = CoapChannel(network_remote, "/test", context)
        asyncio.create_task(self._start_channel())

    async def _start_channel(self):
        await self.channel.start_observe()

    async def push_data(self, payload):
        await self.channel.submit(payload)
