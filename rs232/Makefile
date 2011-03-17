include ../make.inc

TARGETS= ctbtest ctbread

default: ${TARGETS}

ctbtest: ctbtest.o
	$(CC) ${CCFLAGS} -lctb-0.15 $< -o $@
ctbread: ctbread.o
	$(CC) ${CCFLAGS} -lctb-0.15 $< -o $@

%.o: %.cpp
	${CC} $< ${CCFLAGS} -I.  -c -o $@

clean:
	rm -f *.o ${TARGETS}

