#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define PROG_NAME "ReverseIt.c"
#define BUFFER_SIZE 32768
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)	// mode=0644

int main(int argc, char *argv[])
{
	int in_file, out_file;
	char buf1[BUFFER_SIZE];
	char buf2[BUFFER_SIZE];
	int curpos;
	int i;
	char tmp;

	if(argc < 3){
		printf("Usage: %s input_file output_file\n", PROG_NAME);
		exit(0);
	}
	if((in_file=open(argv[1], O_RDONLY)) < 0){
		fprintf(stderr, "Can't open file %s.\n", argv[1]);
		exit(1);
	}
	if((out_file=open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, FILE_MODE)) < 0){
		fprintf(stderr, "Can't open file %s.\n", argv[2]);
		exit(1);
	}
	// curpos = file size
	if((curpos=lseek(in_file, 0, SEEK_END)) == -1){
		fprintf(stderr, "Can't seek file %s.\n", argv[1]);
		exit(1);
	}
	/*
	 * This while loop reads BUFFER_SIZE(or less if not enough)
	 * bytes from in_file backward, reverses it, and stores it
	 * into out_file until all byte in the in_file has processed.
	 * using while + if instead of while is for speeding up.
	 */
	while(curpos >= BUFFER_SIZE){
		lseek(in_file, -BUFFER_SIZE, SEEK_CUR);
		read(in_file, buf1, BUFFER_SIZE);
		lseek(in_file, -BUFFER_SIZE, SEEK_CUR);
		for(i=0; i<BUFFER_SIZE; i++)
			buf2[i] = buf1[BUFFER_SIZE-i-1];
		write(out_file, buf2, BUFFER_SIZE);
		curpos -= BUFFER_SIZE;
	}
	if(curpos){
		lseek(in_file, -curpos, SEEK_CUR);
		read(in_file, buf1, curpos);
		lseek(in_file, -curpos, SEEK_CUR);
		for(i=0; i<curpos; i++)
			buf2[i] = buf1[curpos-i-1];
		write(out_file, buf2, curpos);
	}
	return 0;
}
