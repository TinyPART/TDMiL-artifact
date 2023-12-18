# CoAPerator

Federated Learning device management application for CoAP-enabled clients.

CoAPerator provides a CoAP resource directory service for clients to register
themselves with and starts a communication channel with those clients.

An HTTP+JSON api is provided to interacted with CoAPerator from less constrained
applications. See also the `/docs` or `/redoc` path hosted by CoAPerator for
API docs.

## Prerequisites

Install the python dependencies from the requirements.txt file:

```
pip install -r requirements.txt
```

## Usage

Run the service via:

```
uvicorn main:app  --workers 0
```

## Internals

CoAPerator is a python application combining FastAPI and aiocoap
