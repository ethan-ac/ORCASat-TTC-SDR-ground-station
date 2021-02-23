# User Guide for SDR ground station

## Table of Contents

1. [Introduction](#introduction)
1. [What You'll Need](#what-youll-need)
1. [Variables](#variables)
1. [Blocks](#blocks)
1. [Install necessary parts](#install_necesarry_parts)
    * [SmartRF Studio 7](#smartrf-studio-7)
    * [GNU Radio](#gnu-radio)
    * [OOT Modules](#oot-modules)
1. [How to view/use](#how-to-use/view)
    * [Using ZeroMQ](#using-zeromq)
    * [Viewing running flowgraph](#viewing-running-flowgraph)
    * [Reading GRC terminal](#reading-grc-terminal)
1. [Transmission modes](#transmission-modes)

## Introduction

This user guide aims to provide users with all of the information needed to run the GNU Radio ground station flowgraph. This guide assumes the user has some previous experience with GNU Radio Companion (GRC), GNU Radio, and SmartRF Studio 7. Some basic tutorials for [GNU Radio](https://wiki.gnuradio.org/index.php/Tutorials) and for [SmartRF](https://docs.google.com/document/d/1G3ylXkDHwij8BFPAL0hM6Qb654eDDwRR_HyqPuTKsdM/edit?usp=sharing), or on slides 14-15, 24-27 [here](https://docs.google.com/presentation/d/145syBke3wD0GXqM9OnpUmSf0r15e0uf7wZKPRpoonRI/edit?usp=sharing). A more detailed design document of the GNU Radio ground station flowgraph with explanations of any part of it can be found [here](https://docs.google.com/document/d/1G3ylXkDHwij8BFPAL0hM6Qb654eDDwRR_HyqPuTKsdM/edit?usp=sharing).

## What You'll Need

These are the parts needed to run the GNU Radio ground station flowgraph.

* Computer running Windows that can run SmartRF Studio 7
* Computer running Linux that can run GNU Radio 3.8.
* USRP B210 with antenna
* CC1110 on a functioning board (ie TT&C breakout board) with antenna
* CC Debugger with ribbon cable that comes with it
* USB A to USB B cable
* USB A to USB mini-B cable

## Variables

These are the variable style blocks that control the parameters of the signal processing blocks in the flowgraph.

**Variable blocks --- Variable with an id that can be given to blocks to make changing values used in multiple blocks when the flowgraph is not running easier**
| Block Name | Block Type | Block Unit | Block Description |
| - | - | - | - |
| freq | Real | Hertz | Frequency for USRPs to transmit and receive at |
| samp_rate | Int | Samples/second | Sample rate for most of the flowgraph |
| samp_rate_usrp | Int | Samples/second | Sample rate for the USRPs and adjacent blocks |
| samp_per_sym | Int | Samples/symbol | # samples per symbol for signals transmitted/received |
| bit_per_sym | Int | Bits/symbol | # bits per symbol for signals transmitted/received |
| preamble | String | N/A | 16 bit preamble of packet being encoded |
| access_code | String | N/A | 32 bit access code of packet being encoded |
| pre_tx | Int | Milliseconds | Time to keep pin toggled after data is sent |
| post_tx | Int | Milliseconds | Time to toggle pin before data is sent |

**QT GUI Range blocks --- Variable with an id that can be given to blocks to allow changing values used in multiple blocks when the flowgraph is running**
| Block Name | Block Type | Block Unit | Block Description |
| - | - | - | - |
| tgain | Float | Decibels | Gain of USRP transmitting antenna |
| rgain | Float | Decidels | Gain of USRP receiving antenna |

**QT GUI Check Box block --- Variable with an id that can be given to blocks to allow changing values used in multiple blocks when the flowgraph is running**
| Block Name | Block Type | Block Unit | Block Description |
| - | - | - | - |
| toggle | Bool | N/A | Create check box while flowgraph is running to allow for toggling the mute block, when unchecked it is False, when checked it is True |

## Blocks

These are the blocks used in the flowgraph along with their parameters that concern the SDR ground station.

**Options --- Settings for the flowgraph including id, title, author, output language (language the flowgraph is generated in)**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Title | String | N/A | Name of the flowgraph |
| Author | String | N/A | Author of the flowgraph |
| Output Language | Drop down menu | N/A | Language (Python/C++) the flowgraph will be generated in |
| Generate Options | Drop down menu | N/A | Type of flowgraph that is generated (Graphical/Non-graphical/Hier) |

**ZMQ SUB Message Source --- Gets PDU input from ZMQ socket**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Address | String | N/A | ZeroMQ socket address the block will connect to |
| Timeout | Float | Milliseconds | Data receive timeout |

**Encoder --- Encodes PDU into packet format**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Access Code | String | N/A | 32 bit access code of packet being encoded |
| Bits per Symbol | Int | Bits/symbol | # bits per symbol for signals transmitted/received |
| Samples per Symbol | Int | Samples/symbol | # samples per symbol for signals transmitted/received |
| Preamble | String | N/A | 16 bit preamble of packet being encoded |

**Message Debug --- Prints PDU**

**Modulator --- Converts PDU to tagged stream and GFSK modulates signal**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Samples per Symbol | Int | Samples/symbol | # samples per symbol for signals transmitted/received |
| Decimation | Int | Samples/second | Input port sample rate |
| Interpolation | Int | Samples/second | Output port sample rate |

**Mute --- Connects/Disconnects data stream, enabling/disabling samples passing through this block**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Mute | Bool | N/A | Connects if false, disconnects if true |

**Amp Key --- Toggles Parameter# pin of USB/serial converter**
| Parameter Name | Variable Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Post Tx Delay | Int | Milliseconds | Time to keep pin toggled after data is sent |
| Pre Tx Delay | Int | Milliseconds | Time to toggle pin before data is sent |
| Sample Rate | Int | Samples/second | Sample rate of this hier block |

**QT GUI Frequency Sink --- Displays transmitted signal on a graph of Relative Gain (dB) vs Frequency (MHz)**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Name | String | N/A | Name of the graph |
| FFT Size | Int | Samples | Scope of the displayed graph, higher means more is visible |
| Center Frequency | Real | Hertz | Center frequency of the signal |
| Bandwidth | Real | Hertz | Bandwidth of the signal |

**QT GUI Time Sink --- Displays transmitted signal on a graph of Amplitude (0-1) vs Time (ms)**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Name | String | N/A | Name of the graph |
| Number of Points | Int | Samples | Scope of the displayed graph, higher means more is visible |
| Sample Rate | Float | Samples/second | Sample rate of the signal |
| Autoscale | Drop down menu | N/A | Enable/Disable graph from automatically scaling to have whole signal visible |

**UHD: USRP Sink --- Transmits signal**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Device Address | String | N/A | Serial # of transmitting USRP |
| Sync | Drop down menu | N/A Device the USRP should sync its clock to |
| Samp rate | Real | Samples/second | Sample rate of USRP |
| Ch0: Center Freq | Real | Hertz | Frequency USRP transmits at |
| Ch0: Gain Value | Real | Decibels | Gain of USRP transmitting antenna |
| Ch0: Gain Type | String | N/A | Option of absolute gain in decibels or normalized gain |
| Ch0: Antenna | String | N/A | Antenna USRP will transmit with |
| Ch0: Bandwidth | Real | Hertz | Bandwidth of USRP anti-aliasing filter |

**UHD: USRP Source --- Receives signal**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Device Address | String | N/A | Serial # of transmitting USRP |
| Sync | Drop down menu | N/A | Device the USRP should sync its clock to |
| Samp rate | Real | Samples/second | Sample rate of USRP |
| Ch0: Center Freq | Real | Hertz | Frequency USRP receives at |
| Ch0: AGC | String | N/A | Enable/Disable automatic gain control of the USRP |
| Ch0: Gain Value | Real | Decibels | Gain of USRP receiving antenna |
| Ch0: Gain Type | String | N/A | Option of absolute gain in decibels or normalized gain |
| Ch0: Antenna | String | N/A | Antenna USRP will receive with |
| Ch0: Bandwidth | Real | Hertz | Bandwidth of USRP anti-aliasing filter |

**Demodulator --- Demodulates GFSK modulated signal**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Sample Rate Low Pass | Real | Samples/second | Sample rate of the Low Pass Filter in this hier block |
| Sample Rate Xlating | Real | Samples/second | Sample rate of the Frequency Xlating FFT Filter |

**Decoder --- Converts tagged stream to PDU, decodes the packet and trims to only the length and data fields**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Access Code | String | N/A | 32 bit access code of packet being decoded |

**Print Timestamp --- Prints year-month-day hour:minute:second:millisecond at GMT timezone**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Format | String | Year-month-day hour:minute:second:millisecond | Time units to print |
| Packet counter | Bool | # PDUs | Counts # of PDUs sent through this block |

## Install necessary parts

These are links to all of the necessary programs and resources for the SDR ground station along with instructions on how to install them.

### SmartRF Studio 7

https://www.ti.com/tool/SMARTRFTM-STUDIO

The xml file in the gitlab repo can be installed alongside SmartRF and opened in a SmartRF control panel so the exact setting used during development can be recreated later.

### GNU Radio

Install command for GNU Radio and GNU Radio Companion (GRC)
```
$ sudo apt install gnuradio
```
### OOT Modules

Install command for gr-ampkey
```
$ git clone https://github.com/whateverthislinkendsupbeing/gr-ampkey.git
```
Install command for gr-pduencode
```
$ git clone https://github.com/whateverthislinkendsupbeing/gr-pduencode.git
```
Install command for gr-satellites
```
$ git clone https://github.com/daniestevez/gr-satellites.git
```
Each OOT module must be installed into GNU Radio initially. They must also be updated if any edits are made to them. These are both done from a terminal that is in the desired OOT module's directory.

Both the installing and updating can be done with a custom shell script after copying it into the OOT modules directory and using the following commands.
```
$ sudo ./build.sh
```
Installing can be done manually using the following commands.
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
$ sudo ldconfig
```
Updating a module can be done manually by running the last 4 commands from the build directory in a terminal.

# How to view/use

These are instructions on how to use ZeroMQ, how to view the running flowgraph's graphs effectively, and how to view GRC's terminal outputs.

## Running ZeroMQ

![](/images/zmq_terminal.png)

![](/images/zmq_block.png)

ZeroMQ is used to send strings from a python file running in a terminal to the "ZMQ SUB Message Source" block in the GNU Radio flowgraph. The address in the "ZMQ SUB Message Source" block must match the address in the python file. Multiple blocks or files can be set to the same address, so long as only one of them "binds" to the address and the rest "connect" to the address.

To start the python file, navigate to the directory it's located in from a terminal and run the following command.
```
$ python3 {file_name}.py
```
A running python file can be stopped by inputting Ctrl+C in the terminal it's running in. The python files used in the development of the ground station flowgraph are named packet_tx.py, packet_tx_repeat.py, and receive.py.

packet_tx.py sends a user inputted string.

packet_tx_repeat.py sends a preset string once every second.

receive.py converts received hexadecimal messages into alphabetical characters if possible.

## Viewing running flowgraph

![](/images/grc_running.png)

When the flowgraph is started GRC will open a new window which will contain GUI elements generated by QT GUI blocks.

1. The "QT GUI Check Box" blocks generate a check box which enables/disables the "Mute" block to enable/disable data from passing through it.

1. The "QT GUI Range" blocks generate range sliders and accompanying editable text boxes that can change the gain of the transmitting and receiving antennas of the USRPs.

1. The "QT GUI Time Sink" blocks generate 2 histograms which display the packets as they are transmitted and received.

1. The "QT GUI Frequency Sink" blocks generate 2 frequency graphs that display transmitted and received signals.

The histograms and frequency graphs have control panels on the right to change their viewing settings, all of the settings are fairly intuitive so they will not be covered here. If packets on the "receive" histogram are not appearing centered, 
try moving the delay of the trigger on it. The "send" histogram will only update once it has received enough samples to fill the entire graph, so a few packets may need to be sent before they appear on it. If new QT GUI sinks are added be sure to enable the control panels for the sink by entering properties>config>control panel>enable. To zoom in on the graphs and right click to undo a zoom in.

## Reading GRC terminal

![](/images/grc_terminal.png)

Some blocks in the flowgraph will output to the GRC terminal directly below the flowgraph. The blocks in the ground station flowgraph that do so are the "UHD: USRP Sink/Source", "Decoder", "Message Debug", and "Print timestamp" blocks.

1. The flowgraph prints messages when generating python/c++ flowgraph files and running those files.

1. The "UHD: USRP Source/Sink" blocks prints informative initialization messages upon flowgraph startup which are denoted by "[INFO] [B200]".

1. The "Decoder" hier block, or more specifically the "Correlate Access Code - Tag" block in the "Sync and create packed PDU" hier block in the "Decoder" hier block, prints the access code and bit mask being used in hexadecimal upon flowgraph startup. It also outputs the sample at which it is adding a tag to a data stream when a packet is sent.

1. The "Message Debug" block prints the number of bytes and bytes as hexadecimal of a received packet. In the ground station flowgraph this block does so with the fully encoded packet and the decoded packet after it has been transmitted and received. For the packet that has been transmitted and received it also outputs a message that details the operating frequency, sample rate, and reception time of the USRP used to receive it.

1. The  "Decoder" hier block, or more specifically the "Check CC11xx CTC-16" in the "Decoder" hier block, prints whether or not the CRC16 check for the packet succeeds.

1. The "Print timestamp" block prints the date and time of packets that pass through it and the number of packets that have been sent through it since startup.

## Transmission modes

These are decriptions of the SDR ground stations transmission modes and how to start each of them.

For CC1110 to USRP transmission, start SmartRF in "Packet TX" mode. Then in GRC make sure that the transmission section of the flowgraph is disabled and the reception section is enabled. Start the flowgraph and observe the packets in the GRC terminal as they arrive.

For USRP to CC1110 transmission, start SmartRF in "Packet RX" mode. Then in GRC make sure that the reception section of the flowgraph is disabled and the transmission section is enabled. Start the flowgraph and the packet_tx.py python file, enter characters into the packet_tx.py terminal, and observe the packets in the SmartRF terminal as they arrive.

For loopback USRP to USRP transmission, in GRC make sure both the transmission and reception sections of the flowgraph are enabled. Start the flowgraph and the packet_tx.py python file, enter characters into the packet_tx.py terminal, and observe the packets in the GRC terminal as they arrive.

When transmitting with a USRP, the "ZMQ SUB Message Source", "Encoder", "Message Debug", and "Modulator" blocks can be swapped for a "Signal Source" block to transmit an unmodulated carrier wave.

![](/images/grc_tx_rx_sections.png)

![](/images/smartrf_tx.png)

![](/images/smartrf_rx.png)