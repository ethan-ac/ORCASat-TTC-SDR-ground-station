This repo contains all of the necessary parts to run the GNU Radio ground station with the exception of gr-satellites found [here](https://github.com/daniestevez/gr-satellites)

Document describing how to use this GNU Radio flowgraph can be found [here](https://docs.google.com/document/d/1vxE6jXceIZ9Tap91QvGXx2jc8hywjWRIl0jvHD9sVJg/edit?usp=sharing)

Brief description of each file/folder:

gndstation.grc is the GNU Radio ground station flowgraph

gr-ampkey contains blocks for amplifier keying from GNU Radio using a USB/serial converter

gr-pduencode contains block to encode pdus into packets

zmq contains python scripts to send pdus into the ground station flowgraph

smartrf_gnd_station_setup.xml is the SmartRF studio 7 settings used to have the ground station flowgraph used to interact with a CC1110 on a TT&C breakout dev board
