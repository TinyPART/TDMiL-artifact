import logging
import asyncio
import numpy as np
import cbor2
from aiocoap import Context, Message, GET, PUT, CONTENT

logging.basicConfig(level=logging.INFO)
from functools import reduce

from host_server import update_global_model_by_round


async def fetch_data(client_idx, results, protocol, round_to_agg):
    uri = f"coap://localhost/local_model/c_{client_idx}?round={round_to_agg}"
    request = Message(code=GET, uri=uri, observe=0)

    try:
        protocol_request = protocol.request(request)

        response = await protocol_request.response
        print("First response: %s\n%r" % (response, response.payload))

        async for response in protocol_request.observation:
            print("Next result: %s\n%r" % (response, response.code))

            protocol_request.observation.cancel()
            deserialized_data = cbor2.loads(response.payload)
            model_data = deserialized_data.get("model", [])
            metadata = deserialized_data.get("metadata", {})
            round = int(metadata.get("round", ""))
            num_examples = int(metadata.get("num_examples", ""))
            results.append((np.array(model_data, dtype="object"), num_examples))
            return round
        #     break
        # print("Loop ended, sticking around")
        await asyncio.sleep(1)
    except Exception as e:
        print("Failed to fetch resource:")
        print(e)


async def main(round_to_agg):
    results = []
    protocol = await Context.create_client_context()

    tasks = [
        fetch_data(client_idx, results, protocol, round_to_agg)
        for client_idx in range(num_clients)
    ]
    for coro in asyncio.as_completed(tasks):
        result = await coro
        print(f"Received result: {result}")
        if len(results) >= threshold:
            print("Condition met. Proceeding with further code.")
            break  # Exit the loop
    round = result
    print(f"printing round after fetch:{round}")
    num_examples_total = sum([num_examples for _, num_examples in results])
    print(f"Total examples: {num_examples_total}")
    weighted_weights = [
        [np.array(layer) * num_examples for layer in weights]
        for weights, num_examples in results
    ]

    # Compute average weights of each layer
    weights_prime = [
        reduce(np.add, layer_updates) / num_examples_total
        for layer_updates in zip(*weighted_weights)
    ]
    print(len(results))
    data_and_metadata = {
        "model": [layer.tolist() for layer in weights_prime],
        "metadata": {
            "round": str(round),
        },
    }
    print("printing agg model")
    print(data_and_metadata["model"])

    serialized_data = cbor2.dumps(data_and_metadata)
    success = await update_global_model_by_round(round, serialized_data)

    if success:
        print(f"global model updated for round: {round}")
    else:
        print("failed")


if __name__ == "__main__":
    round_to_agg = 1
    num_clients = 10
    threshold = 6
    asyncio.run(main(round_to_agg))
