# User Guide for SDR ground station

## Table of Contents

1. [Introduction](#introduction)
1. [What You'll Need](#what-youll-need)
1. [Install necessary parts](#install-necessary-parts)
    * [OpenLST TT&C Board](#openlst-ttc-board)
    * [SmartRF Studio 7](#smartrf-studio-7)
    * [USRP B210](#usrp-b210)
    * [GNU Radio](#gnu-radio)
    * [OOT Modules](#oot-modules)
    * [Hier Blocks](#hier-blocks)
    * [OpenLST Python Tools](#openlst-python-tools)
1. [How to view and use](#how-to-view-and-use)
    * [Running ZeroMQ](#running-zeromq)
    * [Viewing running flowgraph](#viewing-running-flowgraph)
    * [Reading GRC terminal](#reading-grc-terminal)
1. [Transmission modes](#transmission-modes)
1. [Variables](#variables)
1. [Blocks](#blocks)

## Introduction

This user guide aims to provide users with all of the information needed to run the GNU Radio ground station flowgraph. This guide assumes the user has some previous experience with GNU Radio Companion (GRC), the OpenLST Vagrant VM, and Linux command terminals. All of the install processes were tested using a fresh install of Ubuntu 20.04 LTS.

Some basic tutorials for [GNU Radio](https://wiki.gnuradio.org/index.php/Tutorials) and for [SmartRF](https://docs.google.com/document/d/1G3ylXkDHwij8BFPAL0hM6Qb654eDDwRR_HyqPuTKsdM/edit?usp=sharing), or on slides 14-15, 24-27 [here](https://docs.google.com/presentation/d/145syBke3wD0GXqM9OnpUmSf0r15e0uf7wZKPRpoonRI/edit?usp=sharing). 

Guide to setting up OpenLST Vagrant VM and TT&C board [here](#https://gitlab.com/ORCASat/ttc/transceiver-poc-firmware/-/blob/master/open-lst/USERS_GUIDE.md).

A more detailed design document of the GNU Radio ground station flowgraph with explanations of any part of it can be found [here](https://docs.google.com/document/d/1G3ylXkDHwij8BFPAL0hM6Qb654eDDwRR_HyqPuTKsdM/edit?usp=sharing).

## What You'll Need

These are the parts needed to run the GNU Radio ground station flowgraph.

* Computer running Windows that can run SmartRF Studio 7
* Computer running Linux (Ubuntu 20.04 recommended) that can run GNU Radio 3.8.2 and the OpenLST Vagrant VM
* Univerisal Software Radio Peripheral (USRP) B210
* USB B cable
* CC1110 on a functioning board (ie TT&C breakout board)
* FTDI TTL-232R-3v3 USB/serial cable
* Antennas or cables to connect USRP and TT&C board
* 2 20DB attenuators
* Texas Instruments (TI) CC Debugger
* USB mini-B cable
* Patch cables
* Power supply

## Install necessary parts

These are links to all of the necessary programs and resources for the SDR ground station along with instructions on how to install them.

### OpenLST TT&C Board

Set up the TT&C board to be used with the GNU Radio flowgraph.

Plug the USB/serial cable from your computer into Uart1 on the TT&C board. Plug the USB mini-B cable from your computer into the CC Debugger and connect the CC Debugger to the TT&C board as is pictured below.

<div align="center">

![](/images/cc_dubber_board_connect.png)

Minimum pin connections for CC Debugger to build and load the bootloader onto a TT&C board

<div align="left">

With either antennas or direct cables, connect the USRPs RFA:Tx/Rx port to the TT&C boards Rx port, and connect the TT&C boards Tx port with 2 20DB attenuators to the USRPs RFA:Rx2 port. Plug the power cables of the TT&C board into the power supply set to 5V/1.2A.

Vagrant up and Vagrant ssh into a OpenLST Vagrant VM and build and load the orcasat_fg radio onto a TT&C board with the aliases "fgbl1" and "fgfw1". This radio has its RF registers set to be identical to the RF resgisters of the CC1110 used when testing the GNU Radio flowgraph with SmartRF. These RF register settings can be seen in the image below.

<div align="center">

![](/images/smartrf_openlst.png)

SmartRF RF registers used for CC1110 on TT&C board

<div align="left">

### SmartRF Studio 7

Download link [here](https://www.ti.com/tool/SMARTRFTM-STUDIO).

The xml file in the gitlab repo should be downloaded alongside SmartRF and opened in a SmartRF control panel so the exact setting used during development can be recreated later.

### USRP B210

Install commands for the USRP B210s software. It is important to install this software before GNU Radio or else the USRP blocks will not be installed. If GNU Radio is installed first, rebuild GNU Radio.

Install USRP B210 dependecies.
```
sudo apt-get install libboost-all-dev libusb-1.0-0-dev doxygen python3-docutils python3-mako python3-numpy python3-requests python3-ruamel.yaml python3-setuptools cmake build-essential
```
Install USRP Hardware Driver (UHD).
```
sudo apt-get install libuhd-dev
```
Plug in the USRP B210 being used now and run the following command. 
```
uhd_find_devices
```
It should result in an error that will ask you to run "~/uhd_images_downloader.py" to download FPGA images for the B210 USRP, do so with a "sudo" in front of it.

<div align="center">

![](/images/uhd_find_devices_error.png)

Error from uhd_find_devices asking to run the FPGA downloader.

<div align="left">

Download USRP FPGA images.
```
sudo /usr/lib/uhd/utils/uhd_images_downloader.py
```
Can check if USRP B210 is installed.
```
uhd_find_devices
```
<div align="center">

![](/images/uhd_find_devices_output.png)

Correct output of "uhd_find_devices"

<div align="left">

Some additional resources on how to install the software for the USRP B210 if it is need:

https://files.ettus.com/manual/page_install.html

https://files.ettus.com/manual/page_build_guide.html

### GNU Radio

Commands for installing GNU Radio 3.8 from the maint-3.8 source branch on GNU Radio's Github page. Keep in mind, during the making of this user guide additional difficulties were encountered during the installation process of GNU Radio 3.8 if other versions or other install methods of GNU Radio previously existed on the computer being used.

Install dependencies for GNU Radio.
```
sudo apt install git cmake g++ libboost-all-dev libgmp-dev swig python3-numpy \
python3-mako python3-sphinx python3-lxml doxygen libfftw3-dev \
libsdl1.2-dev libgsl-dev libqwt-qt5-dev libqt5opengl5-dev python3-pyqt5 \
liblog4cpp5-dev libzmq3-dev python3-yaml python3-click python3-click-plugins \
python3-zmq python3-scipy python3-gi python3-gi-cairo gobject-introspection gir1.2-gtk-3.0
```
Download GNU Radio source code from github repository.
```
git clone https://github.com/gnuradio/gnuradio.git
```
Switch to the maint-3.8 branch of GNU Radio so GNU Radio 3.8 is installed.
```
cd gnuradio
git checkout maint-3.8
git submodule update --init --recursive
```
Build GNU Radio from source. May take upward of 20 minutes.
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DPYTHON_EXECUTABLE=/usr/bin/python3 ../
make -j4
sudo make install
sudo ldconfig
```
Set environmental variables for Python so Out-Of-Tree (OOT) Modules can be installed.

Find \{your-prefix} which is output of the following command. This is the prefix of where GNU Radio installs OOT Modules.
```
gnuradio-config-info --prefix
```
Find \{Py-version} which is the "python#" in output of the following command. This is the Python version being used by GNU Radio to install OOT Modules.
```
find {your-prefix} -name gnuradio | grep "packages"
```
In ~/.basrc and ~/.profile at the home directory, add the following 2 lines at ending.
```
export PYTHONPATH={your-prefix}/lib/{Py-version}/dist-packages:$PYTHONPATH
export LD_LIBRARY_PATH={your-prefix}/lib:$LD_LIBRARY_PATH
```
Open another terminal in the ~/gnuradio/build directory and refresh Linux libraries.
```
sudo ldconfig
```
Restart your computer to apply the changes made.

Some additional resources on how to install GNU Radio and fix errors if it is needed:

https://wiki.gnuradio.org/index.php/InstallingGR

https://wiki.gnuradio.org/index.php/ModuleNotFoundError#B._Finding_the_Python_library

### OOT Modules

Install commands for all of the OOT modules used in the creation of the SDR ground station. OOT modules directories are denoted by the prefix "gr-". If you make changes to the OOT modules and push to this repository, make sure you delete the ~/build directory from the OOT module before doing so. This is because the ~/build directory is specific to your computer and won't work on another persons computer.

Install SDR ground station resources (gr-pduencode, gr-zpdu).
```
git clone https://gitlab.com/ORCASat/ttc/sdr-ground-station.git
```

Each OOT module must be installed into GNU Radio. They must also be updated if any edits are made to them. These are both done from a terminal located at the desired OOT module's directory. After editing and updating an OOT module, GRC must be restarted to apply the changes made.

Install/Update gr-pduencode for GNU Radio
```
cd sdr-ground-station/gr-pduencode
./build.sh
```
Install/Update gr-zpdu for GNU Radio
```
cd sdr-ground-station/gr-zpdu
./build.sh
```
Install gr-satellites and dependencies.
```
sudo apt install python3-pip
pip3 install --user --upgrade construct requests
git clone https://github.com/daniestevez/gr-satellites.git --branch maint-3.8
```
Copy/Paste "build.sh" from ~/sdr-ground-station/gr-pduencode or ~/sdr-ground-station/gr-pduzmq into ~/gr-satellites. Then install/update gr-satellites.
```
cd gr-satellites
./build.sh
```
Installing can also be done manually using the following commands from the desired OOT module's directory.
```
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig
```
Updating a module can be done manually using the following commands from the ~/build directory of the desired OOT module.
```
cmake ..
make
sudo make install
sudo ldconfig
```

### Hier blocks

To make the hier blocks of the SDR ground station available, in GRC open each of the .grc files in ~/sdr-ground-station/gr-pduencode/examples and click the "Generate the flow graph" button on the toolbar. Then open gndstation_hier.grc and the hier block should be present. If not try click the "Reload Blocks" button on the toolbar.

The hier blocks should be available in the "GRC Hier Blocks" tab on the right side of GRC.

To view the flowgraph of a hier block, right click on the hier block and go More > Open Hier.

### OpenLST Python Tools

This is an OPTIONAL part of the SDR ground station currently as it is still being worked on.

OpenLST's Python tools can be installed outside of the Vagrant VM which can allow them to interact with the GNU Radio flowgraph using ZeroMQ.

OpenLST's Python tools were written using Python 2.7 so pip must be installed.
```
sudo apt install curl python
curl https://bootstrap.pypa.io/pip/2.7/get-pip.py --output get-pip.py
sudo python get-pip.py
```
Install dependencies for later. The second command fixes an error where pycrypto does not install and results in an error.
```
python -m pip install --upgrade setuptools wheel
sudo apt-get install build-essential libssl-dev libffi-dev python-dev
```
Install the Python tools package.
```
cd transceiver-poc-firmware/open-lst
python -m pip install -e tools
```
Restart your computer to apply the installations.

# How to view and use

These are instructions on how to use ZeroMQ, how to view the running flowgraph's graphs effectively, and how to view GRC's terminal outputs.

## Running ZeroMQ

<div align="center">

![](/images/zmq_terminal.png)

Terminal commands and outputs for ZeroMQ.

![](/images/zmq_block.png)

Two "ZMQ SUB/PULL Message Source" blocks in GRC.

<div align="left">

ZeroMQ is used as an interface between Python scripts running in terminals and 
to send ascii characters or OpenLST commands from a python script running in a terminal to the "ZMQ SUB Message Source" block in the GNU Radio flowgraph. The address in the "ZMQ SUB Message Source" block must match the address in the python file. Multiple blocks or files can be set to the same address, so long as only one of them "binds" to the address and the rest "connect" to the address.

Start a ZeroMQ Python script.
```
cd sdr-ground-station/zmq-scripts
python3 {file_name}.py
```
A running python script can be stopped by inputting Ctrl+C in the terminal it's running in.

The python scripts included with the SDR ground station are raw_hex_tx.py, raw_rx.py, and raw_tx.py.

| Name | Socket Type | Link Type | Address | Description |
| - | - | - | - | - |
| raw_tx.py | PUB | Bind | tcp://127.0.0.1:66666 | Sends a preset set of hex values once every second. |
| raw_hex_tx.py | PUB | Bind | tcp://127.0.0.1:77777 | Sends a user inputted ascii characters after pressing "Enter". |
| raw_rx.py | SUB | Bind | tcp://127.0.0.1:66666 | Prints received packets as alphanumeric ascii characters if possible or otherwise as hex values. |

## Viewing running flowgraph

<div align="center">

![](/images/grc_running.png)

Running window that appears when GRC flowgraph is started, with some Time and Frequency graphs.

<div align="left">

When the flowgraph is started GRC will open a new window which will contain GUI elements generated by QT GUI blocks.

1. The "QT GUI Check Box" blocks generate a check box which enables/disables the "Mute" block to enable/disable data from passing through it.

1. The "QT GUI Range" blocks generate range sliders and accompanying editable text boxes that can change the gain of the transmitting and receiving antennas of the USRPs.

1. The "QT GUI Time Sink" blocks generate 2 histograms which display the packets as they are transmitted and received.

1. The "QT GUI Frequency Sink" blocks generate 2 frequency graphs that display transmitted and received signals.

The histograms and frequency graphs have control panels on the right to change their viewing settings. If packets on the "receive" histogram are not appearing centered, 
try moving the delay of the trigger. The "send" histogram will only update once it has received enough samples to fill the entire graph, so a few packets may need to be sent before they appear on it. If new QT GUI sinks are added be sure to enable the control panels for the sink by right clicking on a block and clicking on Properties > Config > Control Panel > Yes. To zoom in on any graphs click and drag, and right click to undo a zoom in.

## Reading GRC terminal

<div align="center">

![](/images/grc_terminal.png)

Message output terminal in GRC, with some packets.

<div align="left">

Some blocks in the flowgraph will output to the GRC terminal directly below the flowgraph. The blocks in the ground station flowgraph that do so are the "ZMQ SUB/PULL Message Source", "ZMQ PUB/PUSH Message Sink", "UHD: USRP Sink/Source", "Decoder", "Message Debug", and "Print timestamp" blocks.

1. The flowgraph prints messages when generating python/c++ flowgraph files and running those files.

1. The "ZMQ SUB/PULL Message Source" and "ZMQ PUB/PUSH Message Sink" blocks print the socket type, link type, and address being used by the ZeroMQ socket.

1. The "UHD: USRP Source/Sink" blocks prints initialization information upon flowgraph startup which are denoted by "[INFO] [B200]".

1. The "Correlate Access Code - Tag" block in the "Sync and create packed PDU" hier block in the "Decoder" hier block prints the hex values of the sync word and bit mask being used upon flowgraph startup. It also outputs the sample at which a tag is located in the data stream when a packet is received.

1. The "Message Debug" block prints the length of a packet and the hex values in it. For the packet that has been received by the USRP, it also prints the operating frequency, sample rate, and reception time of the packet.

1. The "Autoswitch" block in the "Decoder" hier block prints whether the packet is structured as a CC1110 packet or a OpenLST RF packet.

1. The "Check CC11xx CTC-16" in the "Decoder" hier block prints whether or not the CRC16 check for the packet succeeds.

1. The "Print timestamp" block prints the date and time of packets that pass through it and the number of packets that have been sent through it since startup.

## Transmission modes

These are decriptions of the SDR ground stations transmission modes and how to start each of them.

For CC1110 to USRP transmission, start SmartRF in "Packet TX" mode. Then in GRC make sure that the transmission section of the flowgraph is disabled and the reception section is enabled. Start the flowgraph and observe the packets in the GRC terminal as they arrive.

For USRP to CC1110 transmission, start SmartRF in "Packet RX" mode. Then in GRC make sure that the reception section of the flowgraph is disabled and the transmission section is enabled. Start the flowgraph and the packet_tx.py python file, enter characters into the packet_tx.py terminal, and observe the packets in the SmartRF terminal as they arrive.

For loopback USRP to USRP transmission, in GRC make sure both the transmission and reception sections of the flowgraph are enabled. Start the flowgraph and the packet_tx.py python file, enter characters into the packet_tx.py terminal, and observe the packets in the GRC terminal as they arrive.

When transmitting with a USRP, the "ZMQ SUB Message Source", "Encoder", "Message Debug", and "Modulator" blocks can be swapped for a "Signal Source" block to transmit an unmodulated carrier wave.

<div align="center">

![](/images/grc_tx_rx_sections.png)

Clearly separated transmission section and reception section of the GRC flowgraph.

![](/images/smartrf_tx.png)

"Packet TX" tab of SmartRF Studio 7.

![](/images/smartrf_rx.png)

"Packet RX" tab of SmartRF Studio 7.

<div align="left">

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

These are the signal processing blocks used in the flowgraph along with their parameters that concern the SDR ground station.

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

To test if the installation was successful connect to openLST's radio_terminal using Zero MQ as its tx-socket and rx-socket.
```
cd transceiver-poc-firmware/open-lst
radio_terminal --rx-socket "tcp://127.0.0.1:55555" --tx-socket "tcp://127.0.0.1:44444" --hwid 0102 --raw
```
Messages can be sent into radio_terminal and displayed as hexadecimal character by typing into the terminal of a running RX_Radio_terminal.py Python script in ~/sdr-ground-station/zmq-scripts.
```
python3 RX_Radio_terminal.py
```
Commands outputs from radio_terminal can be displayed in the terminal of a running TX_Radio_terminal.py Python script in the ~/sdr-ground-station/zmq-scripts directory.
```
$ python3 TX_radio_terminal
```
Messages can be sent from radio_terminal using any of its valid commands.
```
lst get_telem
or
lst ascii hello_there
```
