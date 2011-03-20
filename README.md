Stepper Motor Control
=====================
(Copyright Thomas Hisch 2010-2011)

Overview:
---------

This is a client/server software for controlling certain industrial devices via a serial connection (rs232). At the moment it only supports the commands of the  PM301 controller from McLennan http://www.mclennan.co.uk/ used for controlling 3 servo/stepper motors at the Applied Physics Institute (TU Vienna).

The software is written in  C++, is platform independent and uses the C++ Boost library (http:www.boost.org). The C++ ctb library is used to access the serial port (It currently only supports Windows and Linux). It is intended to replace it with the Boost ASIO library.



Installation:
-------------

Linux (libctb specific):

if you use an USB-RS232 converter (usually such converters have /dev/ttyUSB0 as linux device name) use the devicename as the first argument to ctb::open instead of CTB::COM[1-9].

A nonpriviledged user (non root) needs to have permission to talk to the UART. It needs to be in the same group as /dev/tty* (in fedora this group is called dialout)

to compile the program edit the Makefile if necessary and type

    make

the "rsconf" file contains the serial parameters which are parsed by the server program IAP_server
to run the server (the port used for the TCP connection is hardcoded to 15000 atm) type

    ./IAP_server

start the client with

    ./IAP_client localhost


where you can enter after the promts the commands which are sent to the controller. Type "help" for available/supported commands.

Development:
------------

If you would like to see certain features implemented or support for certain devices, please use githubs issue tracker.


TODO:
-----

* Windows build support
* generalize the client and server programs
* Documentation
* GUI (WxWidgets)
* Linux: libreadline support for the client terminal
