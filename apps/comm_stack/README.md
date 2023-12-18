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
