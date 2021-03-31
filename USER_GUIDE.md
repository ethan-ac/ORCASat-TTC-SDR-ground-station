# User Guide for SDR ground station

## Table of Contents

1. [Introduction](#introduction)
1. [What You'll Need](#what-youll-need)
1. [Setup](#setup)
    * [RF Connections](#rf-connection)
        * [OpenLST TT&C Board](#openlst-ttc-board)
        * [SmartRF Studio 7](#smartrf-studio-7)
    * [USRP B210](#usrp-b210)
    * [GNU Radio](#gnu-radio)
    * [OOT Modules](#oot-modules)
    * [Hier Blocks](#hier-blocks)
    * [OpenLST Python Tools](#openlst-python-tools)
1. [How To View And Use](#how-to-view-and-use)
    * [OpenLST Vagrant VM and TT&C board](#openlst-vagrant-vm-and-ttc-board)
    * [ZeroMQ](#zeromq)
        * [Python Scripts](#python-scripts)
        * [Local OpenLST radio_terminal](#local-openlst-radio_terminal)
    * [Running flowgraph Window](#running-flowgraph-window)
    * [GRC Terminal](#grc-terminal)
1. [Command Transmission Modes](#command-transmission-modes)
    * [Local to TT&C Board](#local-to-ttc-board)
    * [TT&C Board to Local](#ttc-board-to-local)
    * [Loopback](#loopback)
1. [Variables](#variables)
1. [Blocks](#blocks)

## Introduction

This user guide aims to provide users with all of the information needed to run the GNU Radio ground station flowgraph. This guide assumes the user has some previous experience with GNU Radio Companion (GRC), the OpenLST Vagrant VM, and Linux command terminals. All of the install processes were tested using a fresh install of Ubuntu 20.04 LTS.

Some basic tutorials for [GNU Radio](https://wiki.gnuradio.org/index.php/Tutorials) and for [SmartRF](https://docs.google.com/document/d/1G3ylXkDHwij8BFPAL0hM6Qb654eDDwRR_HyqPuTKsdM/edit?usp=sharing), or on slides 14-15, 24-27 [here](https://docs.google.com/presentation/d/145syBke3wD0GXqM9OnpUmSf0r15e0uf7wZKPRpoonRI/edit?usp=sharing). 

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

## Setup

These are links to all of the necessary programs and resources for the SDR ground station along with instructions on how to install them.

### RF Connections

Communicating with the GNU Radio flowgraph over RF can be done with either a TT&C board running OpenLST firmware or a TT&C breakout board running SmartRF Studio 7 firmware.

#### OpenLST TT&C Board

Set up the TT&C board with OpenLST firmware to be used with the GNU Radio flowgraph. Fully detailed user guide available [here](#https://gitlab.com/ORCASat/ttc/transceiver-poc-firmware/-/blob/master/open-lst/USERS_GUIDE.md).

Plug the USB/serial cable from your computer into UART1 on the TT&C board. Plug the USB mini-B cable from your computer into the CC Debugger and connect the CC Debugger to the TT&C board with patch cables as is pictured below.

<div align="center">

![](/images/cc_dubber_board_connect.png)

Minimum pin connections for CC Debugger to build and load the bootloader onto a TT&C board

<div align="left">

With either antennas or direct cables, connect the USRPs RFA:Tx/Rx port to the TT&C boards Rx port, and connect the TT&C boards Tx port with 2 20DB attenuators to the USRPs RFA:Rx2 port. Plug the power cables of the TT&C board into a power supply set to 5V/1.2A.

Install [Vagrant](#https://www.vagrantup.com/) and [VirtualBox with the extension pack](#https://www.virtualbox.org/wiki/Downloads).

On Ubuntu, allow USB devices to be passed to the VM.
```
sudo usermod -aG vboxusers $USER
```
Restart your computer to apply the changes made.

Vagrant up and Vagrant ssh into a OpenLST Vagrant VM and build and load the orcasat_fg radio onto a TT&C board with their aliases (fgblN, fgfwN where N=1/2/3).
```
fgbl1
fgfw1
```
This radio has its RF registers set to be identical to the RF resgisters of the CC1110 used when testing the GNU Radio flowgraph with SmartRF Studio 7. These RF register settings can be seen in the image below.

<div align="center">

![](/images/smartrf_openlst.png)

SmartRF RF registers used for CC1110 on TT&C board

<div align="left">

#### SmartRF Studio 7

Set up the TT&C breakout board with SmartRF Studio 7 to be used with the GNU Radio flowgraph.

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

### Hier Blocks

To make the hier blocks of the SDR ground station available, in GRC open each of the .grc files in ~/sdr-ground-station/gr-pduencode/examples and click the "Generate the flow graph" button on the toolbar. Then open gndstation_hier.grc and the hier block should be present. If not try click the "Reload Blocks" button on the toolbar.

The hier blocks should be available in the "GRC Hier Blocks" tab on the right side of GRC.

To view the flowgraph of a hier block, right click on the hier block and go More > Open Hier.

### OpenLST Python Tools

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

## How To View And Use

These are instructions on how to use ZeroMQ, how to view the running flowgraph's graphs effectively, and how to view GRC's terminal outputs.

### OpenLST Vagrant VM and TT&C board

The OpenLST Vagrant VM allows the sending and receiving of commands over UART1 of the TT&C board. These commands can orignated from the radio_terminal or from the Tx and Rx ports of the board.

Vagrant up and Vagrant ssh into a OpenLST Vagrant VM and open a radio_terminal the alias (rtN where N=1/2/3).
```
radio_terminal --hwid 0001
or
rt1
```
If a radio_terminal --hwid tag is the same hwid as the TT&C board it is on it will send commands to itself. If a radio_terminal --hwid tag is a different hwid from TT&C board it is on it will print any commands that pass the RF frontend. There is no need to have a radio_terminal open on a TT&C board to issue commands to it.

### ZeroMQ

<div align="center">

![](/images/zmq_block.png)

Two "ZMQ SUB/PULL Message Source" blocks in GRC.

<div align="left">

ZeroMQ is used as an interface between the GNU Radio flowgraph and either Python scripts or a locally running OpenLST radio_terminal.

#### Python Scripts

Python scripts can be used to send ascii characters or hex values from a running terminal to a "ZMQ SUB/PULL Message Source" block in the GNU Radio flowgraph. They can also be used to receive ascii characters or hex values from a "ZMQ PUB/PUSH Message Sink" block in the GNU Radio flowgrpah.

To connect any number of ZeroMQ blocks and any number of Python scripts (will be called nodes) to the same ZeroMQ socket, all nodes must follow this criteria:
* The address must be the same. 
* The socket type must be the same, so PUB for data senders with SUB for data receivers, or PUSH for data senders with PULL for data receivers. 
* The link type must be set up so only one node "binds" to the socket and the rest "connect" to the socket.

Start a ZeroMQ Python script.
```
cd sdr-ground-station/zmq-scripts
python3 {file_name}.py
```
A running python script can be stopped by inputting Ctrl+C in the terminal it's running in.

The python scripts included with the SDR ground station are raw_hex_tx.py, raw_rx.py, and raw_tx.py.

| Name | Socket Type | Link Type | Address | Description |
| - | - | - | - | - |
| raw_tx.py | PUB | Connect | tcp://127.0.0.1:66666 | Sends a preset set of hex values once every second. |
| raw_hex_tx.py | PUB | Connect | tcp://127.0.0.1:66666 | Sends a user inputted ascii characters after pressing "Enter". |
| raw_rx.py | SUB | Connect | tcp://127.0.0.1:55555 | Prints received packets as alphanumeric ascii characters if possible or otherwise as hex values. |

#### Local OpenLST radio_terminal

A locally running OpenLST radio_terminal can be used to send its commands to a "ZMQ SUB/PULL Message Source" block in the GNU Radio flowgraph. It can also be used to receive OpenLST's radio_terminal commands from a "ZMQ PUB/PUSH Message Sink" block in the GNU Radio flowgrpah.

Start a local OpenLST radio_terminal.
```
cd transceiver-poc-firmware/open-lst
radio_terminal --rx-socket "tcp://127.0.0.1:55555" --tx-socket "tcp://127.0.0.1:44444" --hwid 0001 --raw
```
* The --rx-socket tag sets the input of the radio_terminal to connect to a ZeroMQ SUB socket with the specified address.
* The --tx-socket tag sets the output of the radio_terminal to connect to a ZeroMQ PUSH socket with the specified address.
* The --hwid tag is the hwid of the device (OpenLST TT&C board) the command is to be sent to.
* The --raw tag replaces all of the sent and received commands with their raw hex values which can be useful for debugging. These are also the values that are sent over the ZeroMQ socket.

If --hwid is set to the hwid of the TT&C board being used it should respond to commands sent to it. If --hwid is set to another value the commands will be printed in the OpenLST Vagrant VMs radio_terminal.

Any valid commands can be sent from radio_terminal.
```
lst ack
or
lst get_telem
or
lst ascii hello_there
```
Currently when the GNU Radio flowgraph and TT&C board are setup to exchange commands, commands sent from a locally running radio_terminal are received by the same local radio_terminal due to the structure of the GNU Radio flowgraph. Responses from the TT&C board are also received, but they will be the second command received in the local radio_terminal.

### Running Flowgraph Window

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

### GRC Terminal

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

## Command Transmission Modes

These are decriptions of the SDR ground stations transmission modes and how to start each of them. This section assumes you have done all of the setup steps.

### Local to TT&C Board

For sending commands from a local radio_terminal to a TT&C board radio_terminal. Start the gndstation_hier.grc flowgraph with all blocks enabled except the "Signal Source" block (used for transmitting umodulated carriers). Start a local OpenLST radio_terminal with the --hwid tag matching the hwid of the TT&C board being used.
```
cd transceiver-poc-firmware/open-lst
radio_terminal --rx-socket "tcp://127.0.0.1:55555" --tx-socket "tcp://127.0.0.1:44444" --hwid 0001
```
When OpenLST commands are typed into the local radio_terminal, in the local radio_terminal an echo of the same command is received by the Rx section of the flowgraph, then a response from the TT&C board is received. This echo can be eliminated by changing tgain=0 rgain=25, but longer commands such as lst get_telem will be corrupted by this fix.

### TT&C Board to Local

For sending commands from a TT&C board radio_terminal to a local radio_terminal. Follow the same procedure as sending commands from local to TT&C board radio_terminal, but start a radio_terminal in the OpenLST Vagrant VM with a --hwid tag that does not match the hwid of the TT&C board being used. The --hwid tag used does not matter since the local radio_terminal does not have a hwid.
```
rt2
```
When OpenLST commands are typed into the TT&C board radio termminal, they will be received by the local radio_terminal and translated into commands. The hex values of the commands will be visible in GRC's terminal and in the graphs of its running window.

### Loopback

For loopback USRP to USRP transmission. Start the gndstation_hier.grc flowgraph with all blocks enabled except the "Signal Source" block (used for transmitting umodulated carriers). Start the raw_hex_tx.py or raw_tx.py Python scripts depending on if you want to send hex values or ascii character. 
```
python3 raw_hex_tx.py
or
python3 raw_tx.py
```
Start the raw_rx.py Python script in another terminal.
```
python3 raw_rx.py
```
Enter ascii characters into the terminal running raw_tx.py, or wait for hex values to be sent if using raw_hex_tx.py. The received data should be printed to the terminal running raw_rx.py. The received data should also be printed to GRC's terminal and in the graphs of its running window.

When transmitting with a USRP, the "ZMQ SUB/PULL Message Source", "OpenLST Encoder", "CC1110 Encoder", "Message Debug", and "Modulator" blocks can be disabled and a "Signal Source" block can be enabled to transmit an unmodulated carrier wave. This should be viewable in the graphs of GRC's running window.

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
| preamble | Int | N/A | Number of preamble bytes of packet being encoded |
| access_code | String | N/A | 32 bit access code of packet being encoded |
| pre_tx | Int | Milliseconds | Time to keep pin toggled after data is sent |
| post_tx | Int | Milliseconds | Time to toggle pin before data is sent |
| cable_file | String | N/A | USB/serial cable file location in Ubuntu |

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

**ZMQ SUB/PULL Message Sink --- Outputs ZeroMQ socket data input as PDU
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Address | String | N/A | ZeroMQ socket address the block will connect to |
| Socket Type | Int | N/A | ZeroMQ socket type the block uses (2 is SUB, 7 is PULL) |
| Link Type | Bool | N/A | ZeroMQ link type the socket uses (bind is true, connect is false) |
| Timeout | Float | Milliseconds | Data receive timeout |

**OpenLST Encoder --- Encodes PDU data into OpenLST RF structured packet**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Bits Per Symbol | Int | Bits/symbol | # bits per symbol for signals transmitted/received |
| Number Of Preamble Bytes | Int | N/A | Length of preamble of packet being encoded in # bytes |
| Samples Per Symbol | Int | Samples/symbol | # samples per symbol for signals transmitted/received |
| Syncword | String | N/A | 32 bit syncword of packet being encoded |
| System | Hex | N/A | Specifies what the OpenLST radio should do with the command in the packet |

**CC1110 Encoder --- Encodes PDU data into CC1110 structured packet**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Bits Per Symbol | Int | Bits/symbol | # bits per symbol for signals transmitted/received |
| Number Of Preamble Bytes | Int | N/A | Length of preamble of packet being encoded in # bytes |
| Samples Per Symbol | Int | Samples/symbol | # samples per symbol for signals transmitted/received |
| Syncword | String | N/A | 32 bit syncword of packet being encoded |

**Message Debug --- Prints inputted PDU**

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
| USB/serial Cable File | String | N/A | USB/serial cable file location in Ubuntu |
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
| Syncword | String | N/A | 32 bit syncword of packet being decoded |

**Print Timestamp --- Prints year-month-day hour:minute:second:millisecond at GMT timezone**
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Format | String | Year-month-day hour:minute:second:millisecond | Time units to print |
| Packet counter | Bool | # PDUs | Counts # of PDUs sent through this block |

**ZMQ PUB/PUSH Message Sink --- Outputs PDU input to ZeroMQ socket
| Parameter Name | Parameter Type | Parameter Unit | Parameter Description |
| - | - | - | - |
| Address | String | N/A | ZeroMQ socket address the block will connect to |
| Socket Type | Int | N/A | ZeroMQ socket type the block uses (1 is PUB, 8 is PUSH) |
| Link Type | Bool | N/A | ZeroMQ link type the socket uses (bind is true, connect is false) |
| Timeout | Float | Milliseconds | Data receive timeout |
