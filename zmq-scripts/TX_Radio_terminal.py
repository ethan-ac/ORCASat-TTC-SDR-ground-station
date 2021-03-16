#Author: Michael Lin
#co- Author: Tim Wriglesworth
 


 #   test TX program



#radio_terminal --rx-socket "tcp://127.0.0.1:55555" --tx-socket "tcp://127.0.0.1:44444" --hwid 0102 --raw


 #!/usr/bin/env python
import zmq
import time
address = "tcp://127.0.0.1:77777"
context = zmq.Context()
socket = context.socket(zmq.PULL)
socket.bind(address)
print("connected to", address)
print("running...")
while True:
    recv = socket.recv()		#print the data radio_terminal sent here
    print(recv)
