include ../make.inc

TARGETS= ctbtest

default: ${TARGETS}

ctbtest: ctbtest.o
	$(CC) ${CCFLAGS} -lctb-0.15 $< -o $@

%.o: %.cpp
	${CC} $< ${CCFLAGS} -I.  -c -o $@

clean:
	rm -f *.o ${TARGETS}

