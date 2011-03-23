include ../make.inc

TARGETS= base

default: ${TARGETS}

base: base.o
	$(CC)  ${MAINO} ${CCFLAGS} `wx-config --libs` -lboost_system -lpthread  $< -o $@


%.o: %.cpp %.hpp
	${CC} $< ${CCFLAGS} -I. `wx-config --cxxflags` -c -o $@


clean:
	rm -f *.o *~ \#*
	rm -f ${TARGETS}
