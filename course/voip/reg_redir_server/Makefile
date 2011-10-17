CC=g++
CFALG=-O9 -Wall
.PHONY: clean

server: server.cpp SockIO.o SipReq.o RegDB.o
	$(CC) $(CFLAG) server.cpp SockIO.o SipReq.o RegDB.o -o server

clean:
	rm -f *.o

RegDB.o: RegDB.cpp
	$(CC) $(CFLAG) -c RegDB.cpp

SipReq.o: SipReq.cpp
	$(CC) $(CFLAG) -c SipReq.cpp

SockIO.o: SockIO.cpp
	$(CC) $(CFLAG) -c SockIO.cpp
