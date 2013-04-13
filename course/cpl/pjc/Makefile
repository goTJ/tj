srcdir =./src
prefix = .

CXX=g++
PROG=pcompiler

.PHONY: all install clean test

all: install

install:
	(cd $(srcdir); \
	make)
	cp src/pcompiler .

clean:
	rm -f $(PROG); \
	(cd $(srcdir); \
	make clean)

test: all
	./test.sh
