import logging
import asyncio
import numpy as np
import cbor2
from aiocoap import Context, Message, GET, PUT
from host_server import update_local_model_by_round, get_global_model_by_round
import argparse

logging.basicConfig(level=logging.INFO)


async def main(client_idx, round=1):
    # read from (round -1) model from server

    success = await get_global_model_by_round(round - 1)
    if success:
        print(f"read global model from round: {round-1}")
    else:
        print(f"failed reading global model from round: {round-1}")
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
    await asyncio.sleep(40 - client_idx)
    input_shape = 10
    hidden_shape = 5
    output_shape = 3

    W1 = np.random.random((hidden_shape, input_shape))
    b1 = np.random.random((hidden_shape))
    W2 = np.random.random((output_shape, hidden_shape))
    # b2 = np.random.random((output_shape))
    b2 = np.array([1, 2, int(client_idx)], dtype=np.float32)

    # Convert the NumPy array to a Python list
    data_and_metadata = {
        "model": [
            W1.tolist(),
            b1.tolist(),
            W2.tolist(),
            b2.tolist(),
        ],
        "metadata": {
            "round": str(round),
            "num_examples": "100",
            "client_id": str(client_idx),
        },
    }

    serialized_data = cbor2.dumps(data_and_metadata)
    request = Message(
        code=PUT,
        payload=serialized_data,
        uri=f"coaps://localhost/local_model/c_{client_idx}?round={round}",
    )

    response = await context.request(request).response

    # print("Result: %s\n%r" % (response.code, response.payload))
    print(f"Result: {response.code}")


async def second_function(client_idx, round=1):
    input_shape = 10
    hidden_shape = 5
    output_shape = 3

    W1 = np.random.random((hidden_shape, input_shape))
    b1 = np.random.random((hidden_shape))
    W2 = np.random.random((output_shape, hidden_shape))
    # b2 = np.random.random((output_shape))
    b2 = np.array([1, 3, 3], dtype=np.float32)

    # Convert the NumPy array to a Python list
    data_and_metadata = {
        "model": [
            W1.tolist(),
            b1.tolist(),
            W2.tolist(),
            b2.tolist(),
        ],
        "metadata": {
            "round": str(round),
            "num_examples": "100",
        },
    }
    # serialized_data = cbor2.dumps(data_and_metadata)
    serialized_data = cbor2.dumps(b"data_and_metadata")
    success = await update_local_model_by_round(client_idx, round, serialized_data)

    if success:
        print(f"local model updated for round: {round}")
    else:
        print("failed")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Asynchronous Client with Argument")

    # Add an argument for client_idx
    parser.add_argument(
        "--client_idx", "-cid", type=int, help="Index of the client", required=True
    )
    parser.add_argument("--round_num", "-round", type=int, help="Round of the client")

    # Parse the command-line arguments
    args = parser.parse_args()

    asyncio.run(main(args.client_idx, args.round_num))
    # asyncio.run(second_function(args.client_idx))
