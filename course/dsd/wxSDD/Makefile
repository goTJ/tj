CC=g++
CFLAGS=-Wall 

main: main.cpp TransTable.o EncodingTable.o qm.o Set.o Pattern.o FormatNormalizer.o
	$(CC) $(CFLAGS) -o $@ main.cpp TransTable.o EncodingTable.o qm.o Set.o Pattern.o FormatNormalizer.o

lib: 

clean:
	-rm -rf *.o

TransTable.o: TransTable.cpp
	$(CC) $(CFLAGS) -c TransTable.cpp

EncodingTable.o: EncodingTable.cpp
	$(CC) $(CFLAGS) -c EncodingTable.cpp

FormatNormalizer.o: FormatNormalizer.cpp
	$(CC) $(CFLAGS) -c FormatNormalizer.cpp

qm.o: qm.cpp
	$(CC) $(FLAG) -c qm.cpp

Set.o: Set.cpp
	$(CC) $(FLAG) -c Set.cpp

Pattern.o: Pattern.cpp
	$(CC) $(FLAG) -c Pattern.cpp
		

