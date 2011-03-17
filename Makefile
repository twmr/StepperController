include make.inc

TARGETS= IAP_server IAP_client_cli

default: ${TARGETS}

MAINO= 	tcp_ip/serversock.o \
	tcp_ip/socket.o \
	IAPBoard.o \
	rs232config.o \
	rs232.o \
	ctb.o

IAP_server: IAP_server.o ${MAINO}
	$(CC)  ${MAINO} ${CCFLAGS} -lctb-0.15 -lboost_regex $< -o $@

IAP_client_cli: IAP_client_cli.o tcp_ip/clientsock.o tcp_ip/socket.o
	$(CC) tcp_ip/clientsock.o tcp_ip/socket.o ${CCFLAGS} $< -o $@


%.o: %.cpp %.hpp
	${CC} $< ${CCFLAGS} -I. -c -o $@


clean:
	rm -f *.o *~ \#*
	rm -f ${TARGETS}
