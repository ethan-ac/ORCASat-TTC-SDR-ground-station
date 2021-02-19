Variables
Blocks
Install necessary parts
SmartRF Studio 7
Ubuntu parts
OOT Modules
How to view/use
Using ZeroMQ
Viewing the running flowgraph
Reading GRC terminal messages
Transmission modes


This user guide aims to provide users with all of the information needed to run the GNU Radio ground station flowgraph. This guide assumes the user has some previous experience with GNU Radio Companion (GRC) and SmartRF Studio 7. Some basic tutorials for these programs exist on slides 14-15, 24-27 here. A more detailed design document of the GNU Radio ground station flowgraph with explanations of any part of it can be found here.
"Variables" and "Blocks" provide a brief description of the purpose of each block in the flowgraph and the purpose of each important variable within those blocks.
"Install necessary parts" provides links and instructions on how to install all of the necessary programs and modules for the GNU Radio ground station flowgraph to run properly.
"How to view/use" gives information on how to run ZeroMQ and how the running window and terminal of GRC are laid out for the GNU Radio ground station flowgraph.
"Transmission modes" details how to run the flowgraph in each of its different modes.


Variables
Options --- Settings for the flowgraph including id, title, author, output language (language the flowgraph is generated in)
Variable --- Variable with an id that can be given to blocks to make changing values used in multiple blocks when the flowgraph is not running easier
freq, real --- Frequency for USRPs to transmit and receive at
samp_rate, int --- Sample rate for most of the flowgraph in samples per second 
samp_rate_usrp, int --- Sample rate for the USRPs and adjacent blocks in samples per second
samp_per_sym, int --- # samples per symbol for signals transmitted/received
bit_per_sym, int --- # bits per symbol for signals transmitted/received
preamble, string --- 16 bit preamble of packet being encoded
access_code, string --- 32 bit access code of packet being encoded
pre_tx, int --- Time to keep pin toggled after data is sent in milliseconds
post_tx, int --- Time to toggle pin before data is sent in milliseconds
QT GUI Range --- Variable with an id that can be given to blocks to allow changing values used in multiple blocks when the flowgraph is running
tgain, float --- Gain of USRP transmitting antenna in decibels
rgain, float --- Gain of USRP receiving antenna in decibels
QT GUI Check Box --- Variable with an id that can be given to blocks to allow changing values used in multiple blocks when the flowgraph is running
toggle, bool --- Create check box while flowgraph is running to allow for toggling the mute block, when unchecked it is False, when checked it is True

Blocks
ZMQ SUB Message Source --- Gets PDU input from ZMQ socket
Address, string --- ZeroMQ socket address the block will connect to
Timeout (msec), float --- Receive timeout in milliseconds
Encoder --- Encodes PDU into packet format
Access Code, string --- 32 bit access code of packet being encoded
Bits per Symbol, int --- # bits per symbol for signals transmitted/received
Samples per Symbol, int --- # samples per symbol for signals transmitted/received
Preamble, string --- 16 bit preamble of packet being encoded
Message Debug --- Prints PDU
Modulator --- Converts PDU to tagged stream and GFSK modulates signal
Samples per Symbol, int --- # samples per symbol for signals transmitted/received
Decimation, int --- Input port sample rate in samples per second
Interpolation, int --- Output port sample rate in samples per second
Mute --- Connects/Disconnects data stream, enabling/disabling samples passing through this block
Mute, bool --- Connects if false, disconnects if true
Amp Key --- Toggles rts# pin of USB/serial converter
Post Tx Delay (ms), int --- Time to keep pin toggled after data is sent in milliseconds
Pre Tx Delay (ms), int --- Time to toggle pin before data is sent in milliseconds
Sample Rate, int --- Sample rate of this hier block in samples per second
UHD: USRP Sink --- Transmits signal
Device Address --- Serial # of transmitting USRP
Sync, options menu --- Device the USRP should sync its clock to
Samp rate (Sps), real --- Sample rate of USRP in samples per second
Ch0: Center Freq (Hz), real --- Frequency USRP transmits at in hertz
Ch0: Gain Value, real --- Gain of USRP transmitting antenna in decibels
Ch0: Gain Type, string --- Option of absolute gain in decibels or normalized gain
Ch0: Antenna, string --- Antenna USRP will transmit with
Ch0: Bandwidth (Hz), real --- Bandwidth of USRP anti-aliasing filter in hertz
QT GUI Frequency Sink --- Displays transmitted signal on a graph of Relative Gain (dB) vs Frequency (MHz)
Name, string --- Name of the graph
FFT Size, int --- Scope of the displayed graph, higher means more is visible
Center Frequency (Hz), real --- Center frequency of the signal in hertz
Bandwidth (Hz), real --- Bandwidth of the signal in hertz
QT GUI Time Sink --- Displays transmitted signal on a graph of Amplitude (0-1) vs Time (ms)
Name, string --- Name of the graph
Number of Points, int --- Scope of the displayed graph, higher means more is visible
Sample Rate, float --- Sample rate of the signal in samples per second
Autoscale, options menu --- Enable/Disable graph from automatically scaling to have whole signal visible
UHD: USRP Source --- Receives signal
Device Address --- Serial # of transmitting USRP
Sync, options menu --- Device the USRP should sync its clock to
Samp rate (Sps), real --- Sample rate of USRP in samples per second
Ch0: Center Freq (Hz), real --- Frequency USRP receives at in hertz
Ch0: AGC, string --- Enable/Disable automatic gain control of the USRP
Ch0: Gain Value, real --- Gain of USRP receiving antenna in decibels
Ch0: Gain Type, string --- Option of absolute gain in decibels or normalized gain
Ch0: Antenna, string --- Antenna USRP will receive with
Ch0: Bandwidth (Hz), real --- Bandwidth of USRP anti-aliasing filter in hertz
QT GUI Frequency Sink --- Displays transmitted signal on a graph of Relative Gain (dB) vs Frequency (MHz)
Name, string --- Name of the graph
FFT Size, int --- Scope of the displayed graph, higher means more is visible
Center Frequency (Hz), real --- Center frequency of the signal in hertz
Bandwidth (Hz), real --- Bandwidth of the signal in hertz
QT GUI Time Sink --- Displays transmitted signal on a graph of Amplitude (0-1) vs Time (ms)
Name, string --- Name of the graph
Number of Points, int --- Scope of the displayed graph, higher means more is visible
Sample Rate, float --- Sample rate of the signal in samples per second
Autoscale, options menu --- Enable/Disable graph from automatically scaling to have whole signal visible
Demodulator --- Demodulates GFSK modulated signal
Sample Rate Low Pass, real --- Sample rate of the Low Pass Filter in this hier block in samples per second
Sample Rate Xlating, real --- Sample rate of the Frequency Xlating FFT Filter in samples per second
Decoder --- Converts tagged stream to PDU, decodes the packet and trims to only the length and data fields
Access Code, string --- 32 bit access code of packet being decoded
Print Timestamp --- Prints year-month-day hour:minute:second:millisecond at GMT timezone
Format, string --- Time units to print (hours/seconds/etc)
Packet counter, bool --- Counts # of bytes in PDUs sent through this block
Message Debug --- Prints PDU


Install necessary parts
SmartRF Studio 7
https://www.ti.com/tool/SMARTRFTM-STUDIO
The xml file in the gitlab repo can be installed alongside SmartRF and opened in a SmartRF control panel so the exact setting used during development can be recreated later.

GNU Radio
Install command for GNU Radio and GNU Radio Companion (GRC)
$ sudo apt install gnuradio
OOT Modules
Install command for gr-ampkey
$ git clone https://github.com/whateverthislinkendsupbeing/gr-ampkey.git
Install command for gr-pduencode
$ git clone https://github.com/whateverthislinkendsupbeing/gr-pduencode.git
Install command for gr-satellites
$ git clone https://github.com/daniestevez/gr-satellites.git
Each OOT module must be installed into GNU Radio initially. They must also be updated if any edits are made to them. These are both done from a terminal that is in the desired OOT module's directory.
Both the installing and updating can be done with a custom shell script after copying it into the OOT modules directory and using the following commands.
$ sudo ./build.sh
Installing can be done manually using the following commands.
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
$ sudo ldconfig
Updating a module can be done manually by running the last 4 commands from the build directory in a terminal.
How to view/use
Running ZeroMQ


ZeroMQ is used to send strings from a python file running in a terminal to the "ZMQ SUB Message Source" block in the GNU Radio flowgraph. The address in the "ZMQ SUB Message Source" block must match the address in the python file. Multiple blocks or files can be set to the same address, so long as only one of them "binds" to the address and the rest "connect" to the address.
To start the python file, navigate to the directory it's located in from a terminal and run the following command.
$ python3 {file_name}.py
A running python file can be stopped by inputting Ctrl+C in the terminal it's running in. The python files used in the development of the ground station flowgraph are named packet_tx.py, packet_tx_repeat.py, and receive.py.
packet_tx.py sends a user inputted string.
packet_tx_repeat.py sends a preset string once every second.
receive.py converts received hexadecimal messages into alphabetical characters if possible.
Viewing the running flowgraph

When the flowgraph is started GRC will open a new window which will contain GUI elements generated by QT GUI blocks.
The "QT GUI Check Box" blocks generate a check box which enables/disables the "Mute" block to enable/disable data from passing through it.
The "QT GUI Range" blocks generate range sliders and accompanying editable text boxes that can change the gain of the transmitting and receiving antennas of the USRPs.
The "QT GUI Time Sink" blocks generate 2 histograms which display the packets as they are transmitted and received.
The "QT GUI Frequency Sink" blocks generate 2 frequency graphs that display transmitted and received signals.
The histograms and frequency graphs have control panels on the right to change their viewing settings, all of the settings are fairly intuitive so they will not be covered here. If packets on the "receive" histogram are not appearing centered, try moving the delay of the trigger on it. The "send" histogram will only update once it has received enough samples to fill the entire graph, so a few packets may need to be sent before they appear on it. If new QT GUI sinks are added be sure to enable the control panels for the sink by entering properties>config>control panel>enable. To zoom in on the graphs and right click to undo a zoom in.

Reading GRC terminal messages

Some blocks in the flowgraph will output to the GRC terminal directly below the flowgraph. The blocks in the ground station flowgraph that do so are the "UHD: USRP Sink/Source", "Decoder", "Message Debug", and "Print timestamp" blocks.
The flowgraph prints messages when generating python/c++ flowgraph files and running those files.
The "UHD: USRP Source/Sink" blocks prints informative initialization messages upon flowgraph startup which are denoted by "[INFO] [B200]".
The "Decoder" hier block, or more specifically the "Correlate Access Code - Tag" block in the "Sync and create packed PDU" hier block in the "Decoder" hier block, prints the access code and bit mask being used in hexadecimal upon flowgraph startup. It also outputs the sample at which it is adding a tag to a data stream when a packet is sent.
The "Message Debug" block prints the number of bytes and bytes as hexadecimal of a received packet. In the ground station flowgraph this block does so with the fully encoded packet and the decoded packet after it has been transmitted and received. For the packet that has been transmitted and received it also outputs a message that details the operating frequency, sample rate, and reception time of the USRP used to receive it.
The  "Decoder" hier block, or more specifically the "Check CC11xx CTC-16" in the "Decoder" hier block, prints whether or not the CRC16 check for the packet succeeds.
The "Print timestamp" block prints the date and time of packets that pass through it and the number of packets that have been sent through it since startup.


Transmission modes
For CC1110 to USRP transmission, start SmartRF in "Packet TX" mode. Then in GRC make sure that the transmission section of the flowgraph is disabled and the reception section is enabled. Start the flowgraph and observe the packets in the GRC terminal as they arrive.
For USRP to CC1110 transmission, start SmartRF in "Packet RX" mode. Then in GRC make sure that the reception section of the flowgraph is disabled and the transmission section is enabled. Start the flowgraph and the packet_tx.py python file, enter characters into the packet_tx.py terminal, and observe the packets in the SmartRF terminal as they arrive.
For loopback USRP to USRP transmission, in GRC make sure both the transmission and reception sections of the flowgraph are enabled. Start the flowgraph and the packet_tx.py python file, enter characters into the packet_tx.py terminal, and observe the packets in the GRC terminal as they arrive.
When transmitting with a USRP, the "ZMQ SUB Message Source", "Encoder", "Message Debug", and "Modulator" blocks can be swapped for a "Signal Source" block to transmit an unmodulated carrier wave.

