#radio_terminal --rx-socket "tcp://127.0.0.1:55555" --tx-socket "tcp://127.0.0.1:44444" --hwid 0102 --raw

#!/usr/bin/env python
import zmq
import time
import pmt

address = "tcp://127.0.0.1:5555"
context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind(address)
print("connected to", address)
print("running...")

while True:
    try:
    	msg = input()
    	pdu = pmt.cons(pmt.PMT_NIL, pmt.init_u8vector(len(msg), msg.encode()))
    	socket.send(pmt.serialize_str(pdu))
    	print(f"sending '{msg}'...")
    	time.sleep(1)
    except KeyboardInterrupt:
    	print ("interrupt received. shutting down.")
    	# clean up
    	socket.close()
    	context.term()
    	exit()
