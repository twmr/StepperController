include make.inc

TARGETS= IAP_server IAP_client_cli

default: ${TARGETS}

MAINO= IAPBoard.o \
	rs232config.o \
	IAPconfig.o \
	rs232.o \
	helper.o \
	ctb.o

IAP_server: IAP_server.o ${MAINO}
	$(CC)  ${MAINO} ${CCFLAGS} -lctb-0.15 -lboost_regex -lboost_thread-mt -lboost_system $< -o $@

IAP_client_cli: IAP_client_cli.o
	$(CC)  ${CCFLAGS} -lboost_system $< -o $@


%.o: %.cpp %.hpp
	${CC} $< ${CCFLAGS} -I. -c -o $@
%.o: %.cpp 
	${CC} $< ${CCFLAGS} -I. -c -o $@


clean:
	rm -f *.o *~ \#*
	rm -f ${TARGETS}
