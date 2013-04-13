#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	int i, n;
	FILE *fin=fopen("in", "w");

	if(argc < 2){
		printf("Usage: ./gen size\n");
		exit(0);
	}
	sscanf(argv[1], "%d", &n);
	for(i=0; i<n; i++)
		fprintf(fin, "j");
	return 0;
}
