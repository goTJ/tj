CC=g++
FLAG=-O9 -Wall
.PHONY: all test clean

all: gen qm check

test: gen qm check
	echo "10" | ./gen > in ; time ./qm < in > out; ./check < out

clean:
	rm -f *.o
	rm -f gen check qm

gen: gen.cpp
	$(CC) $(FLAG) gen.cpp -o gen

qm: qm.cpp Set.o Pattern.o
	$(CC) $(FLAG) qm.cpp Set.o Pattern.o -o qm

Set.o: Set.cpp
	$(CC) $(FLAG) -c Set.cpp

Pattern.o: Pattern.cpp
	$(CC) $(FLAG) -c Pattern.cpp
