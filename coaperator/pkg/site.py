"""CoAP site manager for Coaperator."""
import logging

import aiocoap
import aiocoap.resource
import aiocoap.error
import cbor2
from aiocoap.numbers.contentformat import ContentFormat
import uuid
import pkg.corerd as corerd
from pkg.client import Client
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
        self.add_resource(
            [".well-known", "core"],
            aiocoap.resource.WKCResource(self.get_resources_as_linkheader),
        )
        self.add_resource(["ml"], ModelResource())
        self.rd = corerd.CoreRD(registration=Client, context=self.context)
        self.rd.register_change_callback(_update_callback)
        self.rd.add_resource(self, self.context)
        logging.info(f"initialized CoAP site")


class ModelResource(aiocoap.resource.Resource):
    def render_model(self, uid: uuid.UUID):
        mlstore = MLModelStore()
        model = mlstore.get_model(uid)
        if model is None:
            raise aiocoap.error.NotFound
        payload = model.to_cbor()
        return aiocoap.Message(
            payload=payload,
            code=aiocoap.Code.CONTENT,
            content_format=ContentFormat.CBOR,
        )

    async def render_fetch(self, request):
        payload = request.payload
        try:
            content = cbor2.loads(payload)
        except cbor2.CBORDecodeError:
            return aiocoap.Message(code=aiocoap.Code.NOT_ACCEPTABLE)
        if type(content[0]) is uuid.UUID:
            return self.render_model(content[0])

    async def render_post(self, request):
        payload = request.payload
        try:
            content = cbor2.loads(payload)
        except cbor2.CBORDecodeError:
            return aiocoap.Message(code=aiocoap.Code.NOT_ACCEPTABLE)
        print(f"Received {content} from device {request.remote}")
        devices = coapsite.rd.get_endpoints()
        for device in devices:
            if device.remote == request.remote:
                print(f"Matched {device} with request")
        return aiocoap.Message(code=aiocoap.Code.CHANGED)

    # Correlate to client
    # Store training data

coapsite = CoaperatorSite()
