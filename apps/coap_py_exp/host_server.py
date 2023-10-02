import datetime
import logging

import asyncio

import aiocoap.resource as resource
import aiocoap
import cbor2
import numpy as np
from aiocoap import Context, Message, GET, PUT

# TODO: Clients could not do PUT request on other's behalf
# TODO: GET+observe only when new models are ready


class TimeResource(resource.ObservableResource):
    """Example resource that can be observed. The `notify` method keeps
    scheduling itself, and calles `update_state` to trigger sending
    notifications."""

    def __init__(self):
        super().__init__()

        self.handle = None

    def notify(self):
        self.updated_state()
        self.reschedule()

    def reschedule(self):
        self.handle = asyncio.get_event_loop().call_later(5, self.notify)

    def update_observation_count(self, count):
        if count and self.handle is None:
            print("Starting the clock")
            self.reschedule()
        if count == 0 and self.handle:
            print("Stopping the clock")
            self.handle.cancel()
            self.handle = None

    async def render_get(self, request):
        payload = datetime.datetime.now().strftime("%Y-%m-%d %H:%M").encode("ascii")
        return aiocoap.Message(payload=payload)


class globalModel(resource.ObservableResource):
    def __init__(self):
        super().__init__()
        self.input_shape = 10
        self.hidden_shape = 5
        self.output_shape = 3
        self.W1 = np.random.random((self.hidden_shape, self.input_shape))
        self.b1 = np.random.random((self.hidden_shape))
        self.W2 = np.random.random((self.output_shape, self.hidden_shape))
        self.b2 = np.random.random((self.output_shape))
        self.clients = 3

    async def render_get(self, request):
        python_list = [
            self.W1.tolist(),
            self.b1.tolist(),
            self.W2.tolist(),
            self.b2.tolist(),
        ]

        # Serialize the Python list to CBOR
        self.serialized_data = cbor2.dumps(python_list)
        return aiocoap.Message(payload=self.serialized_data)


class localModelC1(resource.ObservableResource):
    def __init__(self):
        super().__init__()
        self.input_shape = 10
        self.hidden_shape = 5
        self.output_shape = 3
        self.content = None
        self.round = 0
        self.num_examples = 0

    def set_content(self, content):
        self.content = content
        deserialized_data = cbor2.loads(content)
        # Access model data and metadata
        model_data = deserialized_data.get("model", [])
        metadata = deserialized_data.get("metadata", {})

        print(np.array(model_data, dtype="object"))

        # Access specific metadata fields
        self.round = int(metadata.get("round", ""))
        self.num_examples = int(metadata.get("num_examples", ""))

    async def render_get(self, request):
        return aiocoap.Message(payload=self.content)

    async def render_put(self, request):
        # print("PUT payload: %s" % request.payload)
        self.set_content(request.payload)
        return aiocoap.Message(code=aiocoap.CHANGED, payload=self.content)


class localModelC2(resource.ObservableResource):
    def __init__(self):
        super().__init__()
        self.input_shape = 10
        self.hidden_shape = 5
        self.output_shape = 3
        self.content = None
        self.round = 0
        self.num_examples = 0

    def set_content(self, content):
        self.content = content
        deserialized_data = cbor2.loads(content)
        # Access model data and metadata
        model_data = deserialized_data.get("model", [])
        metadata = deserialized_data.get("metadata", {})

        print(np.array(model_data, dtype="object"))

        # Access specific metadata fields
        self.round = int(metadata.get("round", ""))
        self.num_examples = int(metadata.get("num_examples", ""))

    async def render_get(self, request):
        return aiocoap.Message(payload=self.content)

    async def render_put(self, request):
        # print("PUT payload: %s" % request.payload)
        self.set_content(request.payload)
        return aiocoap.Message(code=aiocoap.CHANGED, payload=self.content)


class localModelC3(resource.ObservableResource):
    def __init__(self):
        super().__init__()
        self.input_shape = 10
        self.hidden_shape = 5
        self.output_shape = 3
        self.content = None
        self.round = 0
        self.num_examples = 0

    def set_content(self, content):
        self.content = content
        deserialized_data = cbor2.loads(content)
        # Access model data and metadata
        model_data = deserialized_data.get("model", [])
        metadata = deserialized_data.get("metadata", {})

        print(np.array(model_data, dtype="object"))

        # Access specific metadata fields
        self.round = int(metadata.get("round", ""))
        self.num_examples = int(metadata.get("num_examples", ""))

    async def render_get(self, request):
        return aiocoap.Message(payload=self.content)

    async def render_put(self, request):
        # print("PUT payload: %s" % request.payload)
        self.set_content(request.payload)
        return aiocoap.Message(code=aiocoap.CHANGED, payload=self.content)


# logging setup

logging.basicConfig(level=logging.INFO)
logging.getLogger("coap-server").setLevel(logging.DEBUG)


async def main():
    # Resource tree creation
    root = resource.Site()

    root.add_resource(
        [".well-known", "core"], resource.WKCResource(root.get_resources_as_linkheader)
    )
    root.add_resource(["time"], TimeResource())

    root.add_resource(["global_model"], globalModel())
    root.add_resource(["local_model", "c1"], localModelC1())
    root.add_resource(["local_model", "c2"], localModelC2())
    root.add_resource(["local_model", "c3"], localModelC3())

    await aiocoap.Context.create_server_context(root)

    # Run forever
    await asyncio.get_running_loop().create_future()


if __name__ == "__main__":
    asyncio.run(main())
