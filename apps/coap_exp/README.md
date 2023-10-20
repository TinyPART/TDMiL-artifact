CoAP FL experimental code
=========================

This code starts a simple networked application on native

It first attempts to submit a GET+Observe request to the `/global_model?round=0` endpoint.
Then it will submit CBOR data to the `/local_model/c_{client_id}` endpoint with just metadata

Current application is very minimal with mostly hardcoded values.

The Client ID is configurable in the Makefile
