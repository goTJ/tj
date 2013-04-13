CC=g++
CFLAG=-O9 -Wall

.PHONY: all VQ DPCM LP uninstall

all: VQ DPCM LP

VQ: transmitterVQ receiverVQ

DPCM: transmitterDPCM receiverDPCM

LP: transmitterLP receiverLP

uninstall:
	rm -f transmitterLP transmitterVQ transmitterDPCM receiverLP receiverVQ receiverDPCM trainingVQ

transmitterLP: transmitterLP.cpp lp_config.h
	$(CC) $(CFLAG) transmitterLP.cpp -o transmitterLP

receiverLP: receiverLP.cpp lp_config.h
	$(CC) $(CFLAG) receiverLP.cpp -o receiverLP

transmitterDPCM: transmitterDPCM.cpp
	$(CC) $(CFLAG) transmitterDPCM.cpp -o transmitterDPCM

receiverDPCM: receiverDPCM.cpp
	$(CC) $(CFLAG) receiverDPCM.cpp -o receiverDPCM

transmitterVQ: transmitterVQ.cpp
	$(CC) $(CFLAG) transmitterVQ.cpp -o transmitterVQ

receiverVQ: receiverVQ.cpp
	$(CC) $(CFLAG) receiverVQ.cpp -o receiverVQ

trainingVQ: trainingVQ.cpp
	$(CC) $(CFLAG) trainingVQ.cpp -o trainingVQ
