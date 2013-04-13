/* @JUDGE_ID:   10319NX 686 C */
#include<stdio.h>

int prime[32770];

void initial(void)
{
	int i, j;

	prime[0] = prime[1] = 0;
	for(i=2; i<32770; i++)
		prime[i] = 1;
	for(i=2; i*i<32770; i++)
		if(prime[i])
			for(j=2; j*i<32770; j++)
				prime[i*j] = 0;
}

int main(void)
{
	int n, i, times;

	initial();
	while(scanf("%d", &n) != EOF){
		if(!n)
			return 0;
		times = 0;
		for(i=2; i+i<=n; i++)
			if(prime[i] && prime[n-i])
				times++;
		printf("%d\n", times);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
