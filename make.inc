SYSTEM = $(shell uname)
MACHINE = $(shell uname -n)

BOOST=yes

ifeq ($(MACHINE), firebird)
  CC		= g++
  CCFLAGS       = -Wall -O3
  CCFLAGS       += -std=c++0x -pthread # C++0x standard
endif

ifeq ($(MACHINE), mustang)
  CC            = g++
  CCFLAGS       += -march=amdfam10 -mabm -msse4a -fprefetch-loop-arrays \
                  -combine -funroll-all-loops
  CCFLAGS	+= -std=c++0x -pthread # C++0x standard
endif

ifeq ($(BOOST),yes)
  CCFLAGS       = -DUSE_BOOST
end
