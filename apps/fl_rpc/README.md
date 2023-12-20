CoAP Comm experimental code
=========================

This RIOT application starts an experimental CoAP communication channel with the
CoAPerator application.

It registers itself with the CoAPerator server and allows for setting up the
communication channel. Nothing is implemented on the communication channel at
this point.

## Configuration

- `CORD_ENDPOINT_SINGLETON_ADDRESS`: The IPv6 address of the CoAP service
provided by CoAPerator


## Running the example on RIOT native

Set up networking on the host using the tap script provided by RIOT:

```Shellsession
sudo ./dist/tools/tapsetup/tapsetup -c
```

You now should have one bridge and two tap devices

Set up a router advertisement to provide the devices with a prefix

- Set up a configuration file for radvd

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
uvicorn main:app  --workers 0
```

And start the comm_stack example

```
make -C apps/comm_stack/ term
```

After starting all applications, make sure that the RIOT instance can be reached
via the configured prefix. Verify that it has the prefix via the `ifconfig` shell
command.

The RIOT shell should also automatically start registration on coaperator.
coaperator logs this (exact line might change):

```
INFO:device/RIOT-6581A869BAFF5CA1:New device at 2001:db8::d83c:a5ff:fe9c:3a41
```
