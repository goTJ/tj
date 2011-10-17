CFLAG = -Wall -g
CLIB = -lm
CC = gcc
CCPP = g++
CJAVA = javac

%: %.c
	$(CC) $(CFLAG) $(CLIB) $? -o $@
%: %.cpp
	$(CCPP) $(CFLAG) $? -o $@

%: %.java
	$(CJAVA) $?
