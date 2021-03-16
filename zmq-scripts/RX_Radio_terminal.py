
#Author: Michael Lin
#Co-authour: Tim Wriglesworth

#Test RX program

#radio_terminal --rx-socket "tcp://127.0.0.1:55555" --tx-socket "tcp://127.0.0.1:44444" --hwid 0102 --raw


#!/usr/bin/env python
import zmq
import time
import pmt

address = "tcp://127.0.0.1:66666"
context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind(address)
print("connected to", address)
print("running...")

while True:
    try:
    	# send msg with .send_string()
    	# .send_string() takes a string type as input
    	msg = input()
    	socket.send_string(msg)
    	
    	# send msg with .send()
    	# .send() takes a message_t type as input
    	#msg = input().encode();
    	#socket.send(msg)
    	
    	print(f"sending '{msg}'...")
    	time.sleep(1)
    except KeyboardInterrupt:
    	print ("interrupt received. shutting down.")
    	# clean up
    	socket.close()
    	context.term()
    	exit()
    # or
    # socket.send(bytes(in_str, 'UTF-8'))		#send data to radio_terminal program 
#after running this script, it will say "socket bind success", then press type in the command line, then press enter, it will get muxed to the radio terminal



# #replacement of the following
# context = zmq.Context()
# socket = context.socket(zmq.SUB)
# socket.connect("tcp://127.0.0.2:5559")   
# socket.setsockopt_string(zmq.SUBSCRIBE, '')
# print("running..")

# while True:
# 	string = socket.recv()
# 	print(f"Received: {string}")
