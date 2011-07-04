Stepper Motor Control
=====================
(Copyright Thomas Hisch 2010-2011)

Overview:
---------

This is a client/server software for controlling certain industrial
devices via a serial connection (rs232). At the moment it only
supports the commands of the PM381 controller from
[McLennan](http://www.mclennan.co.uk/) used for controlling 3
servo/stepper motors at the Applied Physics Institute (TU Vienna).

The software is written in C++, is platform independent and uses the
C++ [Boost library](http://www.boost.org). For the communication with
the rs232 interface the C++ ctb library is used access the serial port
(this library currently only supports Windows and Linux).

The GUI (acting as the network client) uses the platform independent
[wxWidgets](http://www.wxwidgets.org) Toolkit, but without the boost
library and the ctb lib.

Installation:
-------------

Linux (libctb specific):

if you use an USB-RS232 converter (usually such converters have
/dev/ttyUSB0 as linux device name) use the devicename as the first
argument to ctb::open instead of CTB::COM[1-9].

A nonpriviledged user (non root) needs to have permission to talk to
the UART. It needs to be in the same group as /dev/tty* (in the linux
distribution Fedora/Redhat this group is called dialout)

Download libctb from
https://iftools.com/download/ctb/0.16/libctb-0.16.tar.gz and install
it according to the Readme file!

to compile the core program edit the Makefile if necessary and type
(in the project root directory)

    make

the "rsconf" file in the root directory of this project contains the
serial parameters which are parsed by the server program *IAP_server*,
and axis.cfg contains all the parameters for each axis. To run the
server (the port used for the TCP connection is hardcoded to 15000
atm) type

    ./IAP_server

start the cli client with

    ./IAP_client localhost [batch-file]

The second argument batch-file is optional. If this file is supplied
then the client sends all commands which are in this file to the
*IAP_server*. The get a list of available commands start the client
without a batch-file and type at the prompt (*#>*) *help*.

 if you prefer the minimalistic GUI client compile it with

    cd wxGUI && make

If this does not work (most likely) then you need to regenerate the
Makefile, do this inside/with DialogBlocks.

Then run the program with (on Linux)
    ./GCCRelease/Project1

Development:
------------

If you would like to see certain features implemented or support for
certain devices, please use
[githubs issue tracker](https://github.com/thisch/StepperController).

Usage Instructions (GUI):
----------------------

Moves/IAP specific stuff:
---------------------------

Allowed Range for each axis:
x:
y:
....

TODO:
-----
* remove Multiplier xml element from parameters.xml
* Windows build support
* generalize the client and server programs
* Documentation
* Linux: libreadline support for the client terminal
