#!/usr/bin/env python3
import uuid

from fastapi import FastAPI, Request, status, HTTPException
import uvicorn
import aiocoap.resource
import aiocoap.error
from aiocoap.numbers.contentformat import ContentFormat
from typing import List, Union
from pkg.site import coapsite
import logging
from models.device import DevModel
from models.ml import MLModel, BaseMLModel, MLTrainingModel
from models.control import BaseControl, ControlResponse
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
    path="/device/{ep}/training/{uid}",
    response_model=MLTrainingModel,
    summary="Retrieve a specific registered device by it's endpoint identifier",
)
async def get_device_training(ep: str, uid: uuid.UUID):
    dev = coapsite.rd.get_endpoint(ep)
    if dev is None:
        raise HTTPException(status_code=404, detail="endpoint not found")
    training = dev.get_training(uid)
    if training is None:
        raise HTTPException(status_code=404, detail="Training model data not found")
    return training


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
    identifier = store.add_model(model)
    return {"identifier": identifier}


@app.get(path="/models", response_model=List[BaseMLModel])
async def get_model_list() -> List[BaseMLModel]:
    store = MLModelStore()
    models = store.list()
    return [BaseMLModel(identifier=uid) for uid in models]

@app.post(path="/control")
async def post_control_data(rpc: BaseControl) -> ControlResponse:
    if  rpc.command == "start" and len(rpc.args) == 1:
        identifier = uuid.UUID(rpc.args[0])
        devices = coapsite.rd.get_endpoints()
        for device in devices:
            await device.mlcontrol.submit_download_model(identifier, None)
    elif rpc.command == "stop" and len(rpc.args) == 1:
        identifier = uuid.UUID(rpc.args[0])
        devices = coapsite.rd.get_endpoints()
        for device in devices:
            print(f"Submitting stop to {device} with {identifier}")
            await device.mlcontrol.submit_upload_training(identifier, None)
    return ControlResponse(reference=0)


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
