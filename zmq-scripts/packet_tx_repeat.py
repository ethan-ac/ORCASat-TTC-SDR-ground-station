#radio_terminal --rx-socket "tcp://127.0.0.1:55555" --tx-socket "tcp://127.0.0.1:44444" --hwid 0102 --raw

#!/usr/bin/env python
import zmq
import time
import pmt

context = zmq.Context()
socket = context.socket(zmq.PUB)
address = "tcp://127.0.0.1:5555"
socket.bind(address)
print("connected to", address)
print("running...")
msg = "hellohellohello"

while(1):
    try:
    	pdu = pmt.cons(pmt.make_dict(), pmt.init_u8vector(len(msg), msg.encode()))
    	socket.send(pmt.serialize_str(pdu))
    	print(f"sending '{msg}'...")
    	time.sleep(1)
    except KeyboardInterrupt:
    	print ("interrupt received. shutting down.")
    	# clean up
    	socket.close()
    	context.term()
    	exit()
