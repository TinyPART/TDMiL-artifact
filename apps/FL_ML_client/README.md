Federated Learning experimental code
====================================

This RIOT application starts an experimental CoAP communication channel with the
CoAPerator application.

It registers itself with the CoAPerator server and allows for setting up the
communication channel. Via this channel the CoAPerator orchestrator can request
status commands and submit commands to the clients

## Configuration

- `CORD_ENDPOINT_SINGLETON_ADDRESS`: The IPv6 address of the CoAP service
provided by CoAPerator


## Running the example on RIOT native

Set up networking on the host using the tap script provided by RIOT:

```Shellsession
sudo RIOT/dist/tools/tapsetup/tapsetup -c
```

You now should have one bridge and two tap devices

Set up a router advertisement to provide the devices with a prefix

- Set up a configuration file for radvd, as found in the `radvd.d` directory of
the repository

```
interface tapbr0
{
        AdvSendAdvert on;
        IgnoreIfMissing on;
        prefix 2001:db8::/64
        {
                AdvOnLink on;
                AdvAutonomous on;
                AdvRouterAddr on;

        };
};
```

- And start radvd with the config:

```
sudo radvd -n -C radvd.d/radvd.conf -p radvd.d/radvd.pid
```

Start the coaperator python application

```
uvicorn main:app  --workers 0 --host ::1
```

And start the FL_ML_client example

```
make -C apps/FL_ML_client/ term
```

After starting all applications, make sure that the RIOT instance can be reached
via the configured prefix. Verify that it has the prefix via the `ifconfig` shell
command.

The RIOT shell should also automatically start registration on coaperator.
coaperator logs this (exact line differs based on the client ID and address):

```
INFO:device/RIOT-6581A869BAFF5CA1:New device at 2001:db8::d83c:a5ff:fe9c:3a41
```

All registered devices and their client identifier with the orchestrator can be
found at the `/devices` endpoint:

```
curl localhost:8000/devices
```

On the CoAPerator, machine learning models can be added via the HTTP+JSON api
using curl:

```
curl -X POST -H 'content-type: application/json' -d @coaperator/tests/iris.json localhost:8000/models
```

This loads the model into the machine learning model store of CoAPerator. The
CoAPerator replies with the identifier of the model.

After this, the registered clients can be instructed to download and start the
model process via the orchestrator.
This is done by sending the start command to the orchestrator together with the
identifier of the model:

```
curl -X POST -H 'content-type: application/json'  localhost:8000/control -d '{ "command": "start", "args": ["c88b1f35761a489796e689921065e176"]}'
```

The FL_ML_client clients registered to the CoAPerator will now receive a command from
CoAPerator to start downloading the model, after which they will start the
model download from the CoAPerator via separate CoAP FETCH requests.
At last the client application should print a line stating that the model has
been downloaded

The training process on the client can be stopped by the orchestrator by
submitting a `stop` RPC to CoAPerator with the model to stop:

```
curl -X POST -H 'content-type: application/json'  localhost:8000/control -d '{ "command": "stop", "args": ["c88b1f35761a489796e689921065e176"]}'
```

All registered clients are now signalled to stop the training process and submit
their training data to the orchestrator.
The training data can be requested from the orchestrator via an endpoint
provided at `/device/<dev_id>/training/<model_uuid>`. Matching the example
above with the client ID:

```
curl localhost:8000/device/RIOT-A5635F88C1662B08/training/c88b1f35-761a-4897-96e6-89921065e176
```

This provides the (dummy) training data provided by the clients
