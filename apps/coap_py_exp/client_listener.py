import logging
import asyncio
import numpy as np
import cbor2
from aiocoap import Context, Message, GET, PUT, CONTENT

logging.basicConfig(level=logging.INFO)
from functools import reduce

from host_server import update_global_model_by_round


async def fetch_client_data(client_idx, results, protocol, round_to_agg):
    uri = f"coap://localhost/local_model/c_{client_idx}?round={round_to_agg}"
    request = Message(code=GET, uri=uri, observe=0)
    # listen for an update on client's endpoint
    try:
        protocol_request = protocol.request(request)

        response = await protocol_request.response
        print("First response: %r" % (response.code))

        print(f"waiting client event for client {client_idx} at round : {round_to_agg}")
        async for response in protocol_request.observation:
            print("Next result: %r" % (response.code))

            protocol_request.observation.cancel()
            deserialized_data = cbor2.loads(response.payload)
            model_data = deserialized_data.get("model", [])
            metadata = deserialized_data.get("metadata", {})
            round = int(metadata.get("round", ""))
            num_examples = int(metadata.get("num_examples", ""))
            client_id = int(metadata.get("client_id", ""))
            results.append((np.array(model_data, dtype="object"), num_examples))
            return round, client_id

        await asyncio.sleep(1)
    except Exception as e:
        print("Failed to fetch resource:")
        print(e)


async def main(num_rounds):
    # trigger initial server listener with dummy data
    serialized_data = cbor2.dumps(b"data_and_metadata")
    success = await update_global_model_by_round(0, serialized_data)

    for r in range(1, num_rounds):
        results = []
        protocol = await Context.create_client_context()

        # local_round = await fetch_server_data(protocol, r)
        # round_to_agg = local_round

        tasks = [
            fetch_client_data(client_idx, results, protocol, r)
            for client_idx in range(num_clients)
        ]
        for coro in asyncio.as_completed(tasks):
            result = await coro
            print(f"Received result: {result}")
            if len(results) >= threshold:
                print("Condition met. Proceeding with further code.")
                break  # Exit the loop
        round = result[0]
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
        await asyncio.sleep(5)
        serialized_data = cbor2.dumps(data_and_metadata)
        success = await update_global_model_by_round(round, serialized_data)

        if success:
            print(f"global model updated for round: {round}")
        else:
            print("failed")


if __name__ == "__main__":
    num_clients = 10
    threshold = 6
    total_rounds = 15
    asyncio.run(main(total_rounds))
