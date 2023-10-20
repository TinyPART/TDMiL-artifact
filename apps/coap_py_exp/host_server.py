import logging

import asyncio

import aiocoap.resource as resource
import aiocoap
import cbor2
import numpy as np
from aiocoap import Context, Message, GET, PUT
import argparse

# TODO: Clients should not do PUT request on other's behalf
# export AIOCOAP_DTLSSERVER_ENABLED=1 


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
        # self.clients = 3
        # initial weights
        data_and_metadata = {
            "model": [
                self.W1.tolist(),
                self.b1.tolist(),
                self.W2.tolist(),
                self.b2.tolist(),
            ],
            "metadata": {"round": "0"},
        }

        self.model = None
        self.initial_data = cbor2.dumps(data_and_metadata)
        self.payload = None
        self.round = None

    def notify(self):
        self.updated_state()

    def set_content(self, content):
        self.payload = content
        deserialized_data = cbor2.loads(content)
        # Access model data and metadata
        model_data = deserialized_data.get("model", [])
        metadata = deserialized_data.get("metadata", {})
        self.model = model_data
        print(np.array(model_data, dtype="object"))

        # Access specific metadata fields
        self.round = int(metadata.get("round", ""))
        self.notify()

    async def render_get(self, request):
        round_param = None
        for option in request.opt.uri_query:
            if option.startswith("round="):
                round_param = int(option.split("=")[1])
                break

        if round_param is not None and round_param == self.round:
            return aiocoap.Message(payload=self.payload)
        else:
            # TODO:not the best way to keep default as 2.05
            return aiocoap.Message(code=aiocoap.CONTENT, payload=b"Round not found")

    async def render_put(self, request):
        round_param = None
        for option in request.opt.uri_query:
            if option.startswith("round="):
                round_param = int(option.split("=")[1])
                break

        if round_param is not None:
            if round_param == 0:
                self.set_content(self.initial_data)
            else:
                self.set_content(request.payload)
            return aiocoap.Message(code=aiocoap.CHANGED, payload=self.payload)
        else:
            # TODO:not the best way to keep default as 2.05
            return aiocoap.Message(code=aiocoap.CONTENT, payload=b"Round not found")


class localModel(resource.ObservableResource):
    def __init__(self, client_id):
        super().__init__()
        self.client_idx = client_id
        self.input_shape = 10
        self.hidden_shape = 5
        self.output_shape = 3
        self.model = None
        self.round = None
        self.num_examples = 0
        self.payload = None

        # self.handle = None
        # self.notify()

    def notify(self):
        self.updated_state()

    def set_content(self, content):
        self.payload = content
        deserialized_data = cbor2.loads(content)
        # Access model data and metadata
        model_data = deserialized_data.get("model", [])
        metadata = deserialized_data.get("metadata", {})

        print(np.array(model_data, dtype="object"))
        print(f"before:{self.round}")
        # Access specific metadata fields
        self.round = int(metadata.get("round", ""))
        print(f"after:{self.round}")
        self.num_examples = int(metadata.get("num_examples", ""))
        self.notify()

    async def render_get(self, request):
        round_param = None
        for option in request.opt.uri_query:
            if option.startswith("round="):
                round_param = int(option.split("=")[1])
                break

        if round_param is not None and round_param == self.round:
            return aiocoap.Message(payload=self.payload)
        else:
            # TODO:not the best way to keep default as 2.05
            return aiocoap.Message(code=aiocoap.CONTENT, payload=b"Round not found")

    async def render_put(self, request):
        round_param = None
        for option in request.opt.uri_query:
            if option.startswith("round="):
                round_param = int(option.split("=")[1])
                break

        if round_param is not None:
            self.set_content(request.payload)
            return aiocoap.Message(code=aiocoap.CHANGED, payload=self.payload)
        else:
            # TODO:not the best way to keep default as 2.05
            return aiocoap.Message(code=aiocoap.CONTENT, payload=b"Round not found")


async def get_local_model_by_round(client_id, round_value):
    uri = f"coaps://localhost/local_model/c_{client_id}?round={round_value}"

    request = Message(code=GET, uri=uri)
    context = await Context.create_client_context()
    context.client_credentials.load_from_dict(
        {
            "coaps://localhost/local_model/*": {
                "dtls": {
                    "psk": b"secretPSK",
                    "client-identity": b"client_Identity",
                }
            }
        }
    )
    response = await context.request(request).response

    if response.code.is_successful():
        return response.payload
    else:
        return None


async def update_local_model_by_round(client_id, round_value, new_data):
    uri = f"coaps://localhost/local_model/c_{client_id}?round={round_value}"

    request = Message(code=PUT, uri=uri, payload=new_data)
    context = await Context.create_client_context()
    context.client_credentials.load_from_dict(
        {
            "coaps://localhost/local_model/*": {
                "dtls": {
                    "psk": b"secretPSK",
                    "client-identity": b"client_Identity",
                }
            }
        }
    )
    response = await context.request(request).response
    print(f"printing update code resp:{response}")

    if response.code == aiocoap.CHANGED:
        return True
    else:
        return False


async def get_global_model_by_round(round_value):
    uri = f"coaps://localhost/global_model?round={round_value}"

    request = Message(code=GET, uri=uri)
    context = await Context.create_client_context()
    context.client_credentials.load_from_dict(
        {
            "coaps://localhost/global_model*": {
                "dtls": {
                    "psk": b"serverPSK",
                    "client-identity": b"server_Identity",
                }
            }
        }
    )
    response = await context.request(request).response

    if response.code.is_successful():
        return response.payload
    else:
        return None


async def update_global_model_by_round(round_value, new_data):
    uri = f"coaps://localhost/global_model?round={round_value}"

    request = Message(code=PUT, uri=uri, payload=new_data)
    context = await Context.create_client_context()
    context.client_credentials.load_from_dict(
        {
            "coaps://localhost/global_model*": {
                "dtls": {
                    "psk": b"serverPSK",
                    "client-identity": b"server_Identity",
                }
            }
        }
    )
    response = await context.request(request).response
    print(f"printing update code resp:{response}")

    if response.code == aiocoap.CHANGED:
        return True
    else:
        return False


# logging setup

logging.basicConfig(level=logging.INFO)
logging.getLogger("coap-server").setLevel(logging.DEBUG)


async def main(num_clients):
    # Resource tree creation
    root = resource.Site()

    root.add_resource(
        [".well-known", "core"], resource.WKCResource(root.get_resources_as_linkheader)
    )

    root.add_resource(["global_model"], globalModel())
    for client_idx in range(num_clients):
        root.add_resource(["local_model", f"c_{client_idx}"], localModel(client_idx))

    server_context = await aiocoap.Context.create_server_context(
        root, bind=("::1", 5683)
    )
    server_context.server_credentials.load_from_dict(
        {
            ":client": {
                "dtls": {
                    "psk": b"secretPSK",
                    "client-identity": b"client_Identity",
                }
            },
            ":server": {
                "dtls": {
                    "psk": b"serverPSK",
                    "client-identity": b"server_Identity",
                }
            }
        }
    )

    # Run forever
    await asyncio.get_running_loop().create_future()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Asynchronous Client with Argument")

    # Add an argument for client_idx
    parser.add_argument(
        "--num_clients", type=int, help="Number of the total clients", default=10
    )

    # Parse the command-line arguments
    args = parser.parse_args()

    asyncio.run(main(args.num_clients))
