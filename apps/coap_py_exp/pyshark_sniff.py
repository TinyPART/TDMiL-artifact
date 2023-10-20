import pyshark
import time

capture = pyshark.LiveCapture(interface='lo')
timeout_seconds = 60  # Adjust this to your desired timeout

start_time = time.time()

for packet in capture.sniff_continuously():
    if 'CoAP' in packet:
        # Check if 'coap' layer exists in the packet
        if 'coap' in packet:
            coap_layer = packet.coap
            print(coap_layer)

    # Check if the timeout has been reached
    if time.time() - start_time >= timeout_seconds:
        break  # Exit the loop when the timeout is reached

# Wait until the timeout expires, and then stop the capture


# Wait until the timeout expires, and then stop the capture


# Wait until the timeout expires, and then stop the capture




# import pyshark

# # Set the TShark path
# pyshark.LiveCapture.tshark_path = '/usr/bin/tshark'  # Update with the correct path

# # Replace 'lo' with the appropriate interface name
# interface = 'lo'

# # Create a Pyshark LiveCapture object with a display filter for CoAP
# capture = pyshark.LiveCapture(interface=interface, display_filter='coap')

# # Start capturing packets
# capture.sniff(timeout=10)  # Capture for 10 seconds (adjust as needed)

# # Iterate through captured packets and print CoAP data
# for packet in capture:
#     if 'CoAP' in packet:
#         print(packet)
