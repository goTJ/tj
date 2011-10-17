/* @JUDGE_ID:   10319NX 10110 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>
#include<math.h>

int main(void)
{
	unsigned int n;
	double nn, ss;

	while(scanf("%u", &n) != EOF){
		if(!n)
			break;
		nn = sqrt(n);
		n = (unsigned int)nn;
		ss = (double)n;
		if(ss == nn)
			printf("yes\n");
		else
			printf("no\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
