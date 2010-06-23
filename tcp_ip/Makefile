include ../make.inc

TARGETS= server_test client_test

default: ${TARGETS}

MAINO= 	clientsock.o \
	serversock.o \
	socket.o \


server_test: server_test.cpp ${MAINO}
	$(CC)  ${MAINO} ${CCFLAGS} $< -o $@


client_test: client_test.cpp ${MAINO}
	$(CC) ${MAINO} ${CCFLAGS} $< -o $@


%.o: %.cpp
	${CC} $< ${CCFLAGS} -I.  -c -o $@


clean:
	rm -f *.o *~ \#*
	rm -f ${TARGETS}
