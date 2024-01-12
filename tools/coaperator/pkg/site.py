import logging

import aiocoap
import aiocoap.resource
import uuid
import pkg.corerd as corerd
from pkg.device import Device
from pkg.mlmodelstore import MLModelStore

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
        self.add_resource(['ml'], ModelResource())
        self.rd = corerd.CoreRD(registration=Device, context=self.context)
        self.rd.register_change_callback(_update_callback)
        self.rd.add_resource(self, self.context)
        logging.info(f"initialized CoAP site")


class ModelResource(aiocoap.resource.Resource, aiocoap.resource.PathCapable):

    async def render_model(self, uid: uuid.UUID):
        mlstore = MLModelStore()
        model = mlstore.get_model(uid)
        payload = model.to_cbor()
        return aiocoap.Message(payload=payload, code=aiocoap.Code.CONTENT)

    async def render(self, request):
        print(request.opt.uri_path)
        path = request.opt.uri_path
        if len(path) < 1:
            return aiocoap.Message(code=aiocoap.Code.NOT_FOUND)

        uid = uuid.UUID(path[0])
        model = MLModelStore().get_model(uid)
        if model is None:
            return aiocoap.Message(code=aiocoap.Code.NOT_FOUND)
        if not request.code == aiocoap.Code.GET:
            raise aiocoap.error.UnsupportedMethod()
        msg = await self.render_model(uid)
        return msg



coapsite = CoaperatorSite()
