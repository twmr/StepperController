#!/bin/bash

g++ $*

err=$?

if [ $1 == '-c' ]; then
    echo ">>> remove $3bj"
    rm -rf "$3bj"
    
    if [ $err == '1' ]; then

	   exit 1

    else

	   exit 0

    fi

fi

