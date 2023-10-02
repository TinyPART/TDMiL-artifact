import logging
import asyncio
import numpy as np
import cbor2
from aiocoap import Context, Message, GET, PUT

logging.basicConfig(level=logging.INFO)
from functools import reduce

num_clients = 3


async def main():
    protocol = await Context.create_client_context()
    results = []
    for client_idx in range(num_clients):
        request = Message(code=GET, uri=f"coap://localhost/local_model/c{client_idx+1}")

        try:
            response = await protocol.request(request).response
        except Exception as e:
            print("Failed to fetch resource:")
            print(e)
        else:
            deserialized_data = cbor2.loads(response.payload)
            # Access model data and metadata
            model_data = deserialized_data.get("model", [])
            metadata = deserialized_data.get("metadata", {})

            # Access specific metadata fields
            round = int(metadata.get("round", ""))
            num_examples = int(metadata.get("num_examples", ""))
            results.append((np.array(model_data, dtype="object"), num_examples))
    num_examples_total = sum([num_examples for _, num_examples in results])
    weighted_weights = [
        [np.array(layer) * num_examples for layer in weights]
        for weights, num_examples in results
    ]

    # Compute average weights of each layer
    weights_prime = [
        reduce(np.add, layer_updates) / num_examples_total
        for layer_updates in zip(*weighted_weights)
    ]
    print(weights_prime)


if __name__ == "__main__":
    asyncio.run(main())
