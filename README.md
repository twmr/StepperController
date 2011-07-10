Stepper Motor Control
=====================
(Copyright Thomas Hisch 2010-2011)

Overview:
---------

This is a FLOSS client/server software for controlling certain industrial
devices via a serial connection (rs232). At the moment it only
supports the commands of the PM381 controller from
[McLennan](http://www.mclennan.co.uk/) used for controlling 5
servo/stepper motors at the Applied Physics Institute (TU Vienna).

The software is written in C++, is platform independent and uses the
C++ [Boost library](http://www.boost.org). For the communication with
the rs232 interface the C++ ctb library is used access the serial port
(this library currently only supports Windows and Linux).

The GUI (acting as the network client) uses the platform independent
[wxWidgets](http://www.wxwidgets.org) Toolkit, but does not depend on
Boost and on libctb.

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

MOVES:
---------

to compile the core program for MOVES create a build directory

    mkdir build && cd build

then start cmake

    cmake -DCMAKE_BUILD_TYPE=Debug ..

and finally type

    make

the configuration file for the hardware is located in the project root
directory and it contains the serial parameters which are parsed by
the server program *moves_server*, it also contains all the parameters
for each axis. To run the server (the port can be set in the XML file)
type

    ./moves_server -f ../parameters.xml

start the cli client from another terminal with

    ./moves_client_cli

to see all accepted program options use the -h or --help command line
option. *moves_client_cli* supports executing batch-files which are
parsed by the server and then sent to the hardware (Use *-b
batchfile*). The get a list of available commands start the client
without a batch-file and type at the prompt (*#>*) *help*.

MOVES GUI:
---------------

In addition to the cli client program a GUI was developed using
wxWidgets, which supports only a subset of the offerd commands of the
PM381 controller. To compile it type

    cd src/gui && make

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
