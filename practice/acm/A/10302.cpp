/* @JUDGE_ID:   10319NX 10302 C++ */
#include<stdio.h>

int main(void)
{
	long long int n;

	while(scanf("%lld", &n) != EOF){
		printf("%lld\n", (n+1)*n*(n-1)*(n-2)/4 + (n+1)*n*(n-1) + (n+1)*n/2);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
