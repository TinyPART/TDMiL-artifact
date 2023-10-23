import logging
import asyncio
import numpy as np
import cbor2
from aiocoap import Context, Message, GET, PUT, CONTENT
import subprocess

logging.basicConfig(level=logging.INFO)

import concurrent
import argparse


def run_command(idx, curr_round):
    try:
        subprocess.run(
            [
                "python",
                "clients_all.py",
                "-cid",
                f"{idx}",
                "-round",
                f"{curr_round+1}",
            ],
            check=True,
        )
        return f"Script {idx} executed successfully."
    except subprocess.CalledProcessError as e:
        return f"Error running script {idx}: {e}"
    except FileNotFoundError as e:
        return f"Error running script {idx}: {e}. Make sure 'python' is in your system's PATH."


def run_commands_in_parallel(num_clients, curr_round):
    with concurrent.futures.ThreadPoolExecutor() as executor:
        for idx in range(num_clients):
            executor.submit(run_command, idx, curr_round)


async def fetch_server_data(protocol, curr_round):
    uri = f"coaps://localhost/global_model?round={curr_round}"
    request = Message(code=GET, uri=uri, observe=0)

    protocol_request = protocol.request(request)
    # listen for an update on server's endpoint

    response = await protocol_request.response
    print("First response: %r" % (response.code))
    print(f"waiting server event at round : {curr_round}")
    async for response in protocol_request.observation:
        print("Next result: %r" % (response.code))

        protocol_request.observation.cancel()
        deserialized_data = cbor2.loads(response.payload)
        model_data = deserialized_data.get("model", [])
        metadata = deserialized_data.get("metadata", {})
        round = int(metadata.get("round", ""))
        # notify all clients to do training
        run_commands_in_parallel(num_clients, curr_round)
        return round + 1


async def main(total_rounds):
    protocol = await Context.create_client_context()
    protocol.client_credentials.load_from_dict(
        {
            "coaps://localhost/global_model*": {
                "dtls": {
                    "psk": b"serverPSK",
                    "client-identity": b"server_Identity",
                }
            }
        }
    )
    for r in range(total_rounds):
        local_round = await fetch_server_data(protocol, r)
        round_to_agg = local_round


if __name__ == "__main__":
    num_clients = 10
    threshold = 6
    parser = argparse.ArgumentParser(description="Asynchronous Client with Argument")

    parser.add_argument(
        "--total_rounds", type=int, help="Number of the total rounds", default=10
    )
    args = parser.parse_args()

    # try:
    #     asyncio.run(main(total_rounds))
    # finally:
    #     # Code to close resources after asyncio.run completes
    #     loop = asyncio.get_event_loop()
    #     loop.close()
    loop = asyncio.get_event_loop()
    try:
        loop.run_until_complete(main(args.total_rounds))
    finally:
        loop.close()
