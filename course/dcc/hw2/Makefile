CFLAG = -O9 -Wall
CLIB = -lm
CC = gcc
CCPP = g++
.PHONY: all

all: transmitter receiver noiseGenServer repeater countPe countEb countSNR

uninstall:
	rm -f transmitter receiver noiseGenServer repeater countPe countEb countSNR
	rm -f *.o

qam.o: qam.cpp
	$(CCPP) $(CFLAG) -c qam.cpp

physical.o: physical.cpp
	$(CCPP) $(CFLAG) -c physical.cpp

transmitter: transmitter.cpp qam.o physical.o
	$(CCPP) $(CFLAG) transmitter.cpp qam.o physical.o -o transmitter

receiver: receiver.cpp qam.o physical.o
	$(CCPP) $(CFLAG) receiver.cpp qam.o physical.o -o receiver

repeater: repeater.cpp qam.o physical.o
	$(CCPP) $(CFLAG) repeater.cpp qam.o physical.o -o repeater

noiseGen.o: noiseGen.cpp
	$(CCPP) $(CFLAG) -c noiseGen.cpp

noiseGenServer: noiseGenServer.cpp noiseGen.o physical.o
	$(CCPP) $(CFLAG) noiseGenServer.cpp noiseGen.o physical.o -o noiseGenServer

countPe: countPe.cpp
	$(CCPP) $(CFLAG) countPe.cpp -o countPe

countEb: countEb.cpp qam.o
	$(CCPP) $(CFLAG) countEb.cpp qam.o -o countEb
