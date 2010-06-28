include make.inc

TARGETS= IAPServer

default: ${TARGETS}

MAINO= 	tcp_ip/serversock.o \
	tcp_ip/socket.o \
	IAPBoard.o \
	ctb.o




IAPServer: IAPServer.o ${MAINO}
	$(CC)  ${MAINO} ${CCFLAGS} -lctb-0.15 $< -o $@


%.o: %.cpp %.hpp
	${CC} $< ${CCFLAGS} -I. -c -o $@


clean:
	rm -f *.o *~ \#*
	rm -f ${TARGETS}
