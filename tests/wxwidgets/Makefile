include ../../make.inc

TARGETS= base

default: ${TARGETS}

base: base.o
	$(CC)  ${MAINO} ${CCFLAGS} `wx-config --libs` $< -o $@


%.o: %.cpp %.h
	${CC} $< ${CCFLAGS} -I. `wx-config --cxxflags` -c -o $@


clean:
	rm -f *.o *~ \#*
	rm -f ${TARGETS}
