#!/usr/bin/env python3
import uuid

from fastapi import FastAPI, Request
import uvicorn
import aiocoap.resource
import aiocoap.error
from aiocoap.numbers.contentformat import ContentFormat
from typing import List
from pkg.site import coapsite
import logging
from models.device import DevModel
from models.ml import MLModel, BaseMLModel
from pkg.mlmodelstore import MLModelStore

app = FastAPI(title="coaperator")

logging.basicConfig()
logging.getLogger("").setLevel("INFO")


@app.get("/")
async def read_root():
    return {"hello": "world"}


@app.get(
    "/devices",
    response_model=List[DevModel],
    summary="Retrieve the list of devices registered",
)
def read_devices():
    devices = coapsite.rd.get_endpoints()
    return [DevModel.from_device(device) for device in devices]


@app.get(
    path="/device/{ep}",
    response_model=DevModel,
    summary="Retrieve a specific registered device by it's endpoint identifier",
)
async def get_device(ep: str):
    dev = coapsite.rd.get_endpoint(ep)
    return DevModel.from_device(dev)


@app.get(
    path="/models/{uid}",
    response_model=MLModel,
    summary="Retrieve a specific model by ID",
)
async def get_model(uid: uuid.UUID):
    return MLModelStore().get_model(uid)


@app.post(path="/models")
async def submit_model(model: MLModel):
    store = MLModelStore()
    store.add_model(model)


@app.get(path="/models", response_model=List[BaseMLModel])
async def get_model_list() -> List[BaseMLModel]:
    store = MLModelStore()
    models = store.list()
    return [BaseMLModel(identifier=uid) for uid in models]


class Welcome(aiocoap.resource.Resource):
    async def render_get(self, request):
        try:
            return aiocoap.Message(
                payload="hello world", content_format=ContentFormat.TEXT
            )
        except KeyError:
            raise aiocoap.error.UnsupportedContentFormat


@app.on_event("startup")
async def main_process_start():
    await coapsite.init()


if __name__ == "__main__":
    uvicorn.run(app, host="::", port=8000)
