import logging
import asyncio
import numpy as np
import cbor2
from aiocoap import Context, Message, GET, PUT

logging.basicConfig(level=logging.INFO)
#TODO argparse

async def main():
    context = await Context.create_client_context()

    await asyncio.sleep(2)
    input_shape = 10
    hidden_shape = 5
    output_shape = 3

    W1 = np.random.random((hidden_shape, input_shape))
    b1 = np.random.random((hidden_shape))
    W2 = np.random.random((output_shape, hidden_shape))
    # b2 = np.random.random((output_shape))
    b2 = np.array([1, 1, 3], dtype=np.float32)

    # Convert the NumPy array to a Python list
    data_and_metadata = {
        "model": [
            W1.tolist(),
            b1.tolist(),
            W2.tolist(),
            b2.tolist(),
        ],
        "metadata": {
            "round": "1",
            "num_examples": "100",
        },
    }

    serialized_data = cbor2.dumps(data_and_metadata)
    request = Message(
        code=PUT, payload=serialized_data, uri="coap://localhost/local_model/c0?round=1"
    )

    response = await context.request(request).response

    print("Result: %s\n%r" % (response.code, response.payload))


if __name__ == "__main__":
    asyncio.run(main())
