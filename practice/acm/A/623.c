/*   @JUDGE_ID:   10319NX   623   C */
#include<stdio.h>
#include<string.h>

enum { LEN = 1000000, MAX = 10 };
unsigned int sub[LEN];
unsigned int lenth;

void mul( unsigned int i)
{
	unsigned int j, buf = 0;

	for(j=0; j<lenth; j++){
		sub[j] = sub[j]*i + buf;
		buf = sub[j]/MAX;
		sub[j] %= MAX;
		if( j==lenth-1 && buf>0)
			lenth++;
	}
}

void output(int n)
{
	unsigned int jjj = lenth-1;
	int i, fly;

	printf("%d!\n", n);
	printf("%u", sub[jjj]);
	for(i=jjj-1; i>-1; i--)
		printf("%01u", sub[i]);
	printf("\n");
}

int main(void)
{
	unsigned int i, n;

	while( scanf("%u", &n) != EOF){
		lenth = sub[0] = 1;
		for(i=1; i<=n; i++)
			mul( i);
		output(n);
		memset(sub, 0, sizeof(sub));
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
