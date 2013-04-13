CFLAG = -O9 -Wall
CC = gcc

.PHONY: all

all: mod_client mod_server

mod_client: mod_client.c
	$(CC) $(CFLAG) $? -o $@

mod_server: mod_server.c
	$(CC) $(CFLAG) $? -o $@
