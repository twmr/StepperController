SYSTEM = $(shell uname)
MACHINE = $(shell uname -n)

BOOST=yes
CPP_0x=yes

ifeq ($(MACHINE), firebird)
  CC		= g++
  CCFLAGS       = -Wall -O3
  ifeq ($(CPP_0x),yes)
    CCFLAGS     += -std=c++0x -pthread
  endif
endif

ifeq ($(MACHINE), mustang)
  CC            = g++
  CCFLAGS       += -march=amdfam10 -mabm -msse4a -fprefetch-loop-arrays \
                  -combine -funroll-all-loops
  ifeq ($(CPP_0x),yes)
    # C++0x standard
    CCFLAGS     += -std=c++0x -pthread
  endif
endif

ifeq ($(BOOST),yes)
  CCFLAGS       += -DUSE_BOOST
endif
