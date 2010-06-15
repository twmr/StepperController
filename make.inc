SYSTEM = $(shell uname)
MACHINE = $(shell uname -n)

ifeq ($(MACHINE), firebird)
  CC		= g++
  CCFLAGS       =
endif

ifeq ($(MACHINE), mustang)
  CC            = g++
  CCFLAGS       += -march=amdfam10 -mabm -msse4a -fprefetch-loop-arrays \
                  -combine -funroll-all-loops
endif
