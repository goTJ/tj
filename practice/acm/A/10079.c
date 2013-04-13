/*   @JUDGE_ID:   10319NX   10079   C */
#include<stdio.h>

int main(void)
{
	long long int n;

	while(scanf("%lld",&n) && n>=0)
		printf("%lld\n",n*(n+1)/2+1);
	return 0;
}
/* @END_OF_SOURCE_CODE */
