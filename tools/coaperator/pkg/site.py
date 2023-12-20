import logging

import aiocoap
import aiocoap.resource
import pkg.corerd as corerd
from pkg.device import Device


def _update_callback(*args):
    print(args)


class CoaperatorSite(aiocoap.resource.Site):
    def __init__(self):
        super().__init__()
        self.rd = None
        self.context = None

    async def init(self):
        self.context = await aiocoap.Context.create_server_context(self)
        self.add_resource(['.well-known', 'core'], aiocoap.resource.WKCResource(self.get_resources_as_linkheader))
        self.rd = corerd.CoreRD(registration=Device, context=self.context)
        self.rd.register_change_callback(_update_callback)
        self.rd.add_resource(self, self.context)
        logging.info(f"initialized CoAP site")


coapsite = CoaperatorSite()
