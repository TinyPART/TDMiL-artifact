# CoAPerator

Federated Learning device management application for CoAP-enabled clients.

CoAPerator provides a CoAP resource directory service for clients to register
themselves with and starts a communication channel with those clients.

An HTTP+JSON api is provided to interacted with CoAPerator from less constrained
applications. See also the `/docs` or `/redoc` path hosted by CoAPerator for
API docs.

## Prerequisites

Installation can be done using Poetry:
```
poetry install --only main --no-root
```

## Usage

Run the service via:

```
uvicorn main:app  --workers 0
```

## Internals

CoAPerator is a python application combining FastAPI and aiocoap

## Development

Install development dependencies via:
```
poetry install --no-root
```

Run the linters using:
```
poetry run black .
poetry run flake8 .
poetry run mypy .
```
