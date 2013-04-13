mysort.o: mysort.c
	gcc -O3 -g -Wall -c mysort.c

main: main.c mysort.o
	gcc -O3 -g -Wall main.c mysort.o -o main

kth: kth.c mysort.o
	gcc -O3 -g -Wall kth.c mysort.o -o kth
