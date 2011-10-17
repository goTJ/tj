CFLAG = -O9
CLIB = -lm
CC = gcc
CCPP = g++

.PHONY: all clean

all: server RdtClient

server: server.cpp library.o
	$(CCPP) $(CFLAG) server.cpp library.o -o server

RdtClient: RdtClient.c library.o
	$(CCPP) $(CFLAG) RdtClient.c library.o -o RdtClient

library.o: library.cpp
	$(CCPP) $(CFLAG) -c library.cpp

clean:
	rm -f *.o

# only for test
test_sender: test_sender.cpp library.o
	$(CCPP) $(CFLAG) test_sender.cpp library.o -o test_sender

