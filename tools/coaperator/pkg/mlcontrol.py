import aiocoap
import cbor2 as cbor
from pkg.channel import CoapChannel


MLCONTROL_RPC_STATUS = 0
MLCONTROL_RPC_START = 1
MLCONTROL_RPC_STOP = 2


def cbor2_dumps_kwargs():
    return {'datetime_as_timestamp': True, 'canonical': True}


class MLController(object):

    def __init__(self, channel: CoapChannel):
        self.channel = channel

    @staticmethod
    def _fmt_status():
        return cbor.dumps([int(MLCONTROL_RPC_STATUS)], **cbor2_dumps_kwargs())

    async def get_status(self):
        payload = self._fmt_status()
        response = await self.channel.submit(payload)
        if response:
            if response.code == aiocoap.Code.CONTENT and response.payload:
                return cbor.loads(response.payload)


