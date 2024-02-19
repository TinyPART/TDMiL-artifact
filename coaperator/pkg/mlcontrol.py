import uuid

import aiocoap
import cbor2 as cbor
from pkg.channel import CoapChannel
from typing import List, Optional


MLCONTROL_RPC_STATUS = 0
MLCONTROL_RPC_START = 1
MLCONTROL_RPC_STOP = 2
MLCONTROL_RPC_MODEL_FETCH = 3
MLCONTROL_RPC_MODEL_STOP_POST = 4


def cbor2_dumps_kwargs():
    return {"datetime_as_timestamp": True, "canonical": True}


class MLController(object):
    def __init__(self, channel: CoapChannel):
        self.channel = channel

    @staticmethod
    def _fmt_status():
        return cbor.dumps([int(MLCONTROL_RPC_STATUS)], **cbor2_dumps_kwargs())

    @staticmethod
    def _fmt_download_model(identifier: uuid.UUID, layers: List[str]):
        return cbor.dumps(
            [int(MLCONTROL_RPC_MODEL_FETCH),
             [
                 identifier,
                 layers
             ]
             ]
            , **cbor2_dumps_kwargs())

    @staticmethod
    def _fmt_upload_model(identifier: uuid.UUID, layers: List[str]):
        return cbor.dumps(
            [int(MLCONTROL_RPC_MODEL_STOP_POST),
             [
                 identifier,
                 layers
             ]
             ]
            , **cbor2_dumps_kwargs())


    async def get_status(self):
        payload = self._fmt_status()
        response = await self.channel.submit(payload)
        if response:
            if response.code == aiocoap.Code.CHANGED and response.payload:
                return cbor.loads(response.payload)

    async def submit_download_model(self, identifier: uuid.UUID, layers: Optional[List[str]]):
        layers = layers or []
        payload = self._fmt_download_model(identifier, layers)
        response = await self.channel.submit(payload)
        return response

    async def submit_upload_training(self, identifier: uuid.UUID, layers: Optional[List[str]]):
        layers = layers or []
        payload = self._fmt_upload_model(identifier, layers)
        response = await self.channel.submit(payload)
        return response
