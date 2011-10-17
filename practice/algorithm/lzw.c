#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BIT 12
#define LIMIT (1 << BIT)-1
#define EndOfFile (1 << BIT)-1
#define SIZE 5021	/* a prime greater than 2^Bit, for Hash */

enum MESSAGE{ Normal, OutOfMemory, FileError, FormatError};

typedef struct Code{
	unsigned int prefix;
	unsigned int append;
}Code;

Code *code;
int *hash;

int initialEncode(void)
{
	code = (Code *)malloc(SIZE*sizeof(Code));
	hash = (int *)malloc(SIZE*sizeof(int));
	if(code==NULL || hash==NULL)
		return 1;
	memset(hash, 0, sizeof(hash));
	memset(code, 0, sizeof(code));
	return 0;
}

int getHash(unsigned int prefix, unsigned int append)
{
	int label=(prefix*92871+(int)append)%SIZE;

	while(1){
		if(!hash[label])
			return label;
		if(code[hash[label]].prefix==prefix && code[hash[label]].append==append)
			return label;
		label += append+1;
		label %= SIZE;
	}
}

void outputCode(unsigned int code, FILE *fout)
{
	static int bit=0;
	static unsigned long buffer=0L;

	buffer |= (unsigned long)code << (32-BIT-bit);
	bit += BIT;
	while(bit >= 8){
		putc(buffer>>24, fout);
		buffer <<= 8;
		bit -= 8;
	}
}

int encode(FILE *fin, FILE *fout)
{
	int limCode=256;
	unsigned int prefix;
	unsigned char append;
	int hashIndex;

	if(initialEncode())
		return OutOfMemory;
	prefix = getc(fin);
	while(!feof(fin)){
		append = getc(fin);
		hashIndex = getHash(prefix, append);
		if(hash[hashIndex]){
			prefix = hash[hashIndex];
		}
		else{
			outputCode(prefix, fout);
			if(limCode < LIMIT){
				hash[hashIndex] = limCode;
				code[limCode].prefix = prefix;
				code[limCode++].append = append;
			}
			prefix = (unsigned int)append;
		}
	}
	outputCode(prefix, fout);
	outputCode(EndOfFile, fout);
	outputCode(0, fout);
	return Normal;
}

int initialDecode(void)
{
	code = (Code *)malloc(SIZE*sizeof(Code));
	if(code==NULL)
		return 1;
	memset(code, 0, sizeof(code));
	return 0;
}

unsigned int readCode(FILE *fin)
{
	static int bit=0;
	static unsigned long buffer=0L;
	unsigned int code;

	while(bit < BIT){
		if(feof(fin)){
			fprintf(stderr, "It's Not LZW File Format\n");
			exit(FormatError);
		}
		buffer |= (unsigned long) getc(fin) << (24-bit);
		bit += 8;
	}
	code = buffer>>(32-BIT);
	buffer <<= BIT;
	bit -= BIT;
	return code;
}

char outputChar(unsigned int prefix, FILE *fout)
{
	static unsigned char head;

	if(prefix >= LIMIT){
		fprintf(stderr, "It's Not LZW File Format\n");
		exit(FormatError);
	}
	else if(prefix >= 256){
		head = outputChar(code[prefix].prefix, fout);
		putc(code[prefix].append, fout);
		return head;
	}
	else{
		putc(prefix, fout);
		return (char)prefix;
	}
}

int decode(FILE *fin, FILE *fout)
{
	int limCode=256;
	unsigned int prefix, last;
	unsigned char head;

	if(initialDecode())
		return OutOfMemory;
	head = readCode(fin);
	putc(head, fout);
	last = head;
	while((prefix=readCode(fin)) != EndOfFile){
		if(prefix >= limCode){
			outputChar(last, fout);
			putc(head, fout);
		}
		else{
			head = outputChar(prefix, fout);
		}
		if(limCode < LIMIT){
			code[limCode].prefix = last;
			code[limCode++].append = head;
		}
		last = prefix;
	}
	return Normal;
}

void helpPage(void)
{
	printf("Usage:\t\n");
	printf("\tlzw -e filename\t(Encode filename to filename.lzw)\n");
	printf("\tlzw -d filename\t(Decode filename.lzw to decode.out)\n");
}

int main(int argv, char **argc)
{
	FILE *fin, *fout;
	char buf[20];

	if(argv < 2){
		helpPage();
		return 0;
	}
	if(!strcmp(argc[1], "-e")){
		sprintf(buf, "%s.lzw", argc[2]);
		fin=fopen(argc[2], "rb");
		fout = fopen(buf, "wb");
		if(fin==NULL || fout==NULL){
			fprintf(stderr, "Opening File Error!!\n");
			return FileError;
		}
		return encode(fin, fout);
	}
	else if(!strcmp(argc[1], "-d")){
		fin=fopen(argc[2], "rb");
		fout = fopen("decode.txt", "wb");
		if(fin==NULL || fout==NULL){
			fprintf(stderr, "Opening File Error!!\n");
			return FileError;
		}
		return decode(fin, fout);
	}
	else{
		helpPage();
	}
	return Normal;
}
