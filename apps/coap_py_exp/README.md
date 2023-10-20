### Python CoAP experiments for FL

#### Funtional prototype for communication using events
- There is host server hosting CoAP endpoints; run by (configured for 10 clients but adjustable)
-For DTLS setup, environment variable is set for `host_server` setting through `export AIOCOAP_DTLSSERVER_ENABLED=1` in terminal followed by
```bash
python host_server.py 
```
- Server event listener listens any update of global model and informs clients to initial training based on previous aggregated updates
```bash
python server_listener.py 
```
- Client evvet listener listens for clients' updates to trigger aggregation
```bash
python client_listener.py
```

PS: run the above scripts in different terminal in above sequence.