//### PROGRAM
/*
ID: flyherm003
PROG: barn1.c
*/
#include<stdio.h>
#include<stdlib.h>

enum { MAX = 200 };

int comp(const void *a, const void *b)
{
	return *(int *)b - *(int *)a;
}

int main(void)
{
	FILE *in = fopen("barn1.in", "r");
	FILE *out = fopen("barn1.out", "w");
	int m, s, c, i, lenth;
	int cow[MAX];
	
	while(fscanf(in, "%d%d%d", &m, &s, &c) == 3){
		lenth = 0;
		for(i=0; i<c; i++)
			fscanf(in, "%d", cow+i);
		qsort(cow, c, sizeof(cow[0]), comp);
		for(i=0; i<c-1; i++)
			cow[i] = cow[i] - cow[i+1] - 1;
		qsort(cow, c-1, sizeof(cow[0]), comp);
		for(i=m-1; i<c-1; i++)
			lenth += cow[i];
		lenth += c;
		fprintf(out, "%d\n", lenth);
	}
	return 0;
}
//### END
