/*   @JUDGE_ID:   10319NX   382   C */
#include<stdio.h>

int judge(int n)
{
	int i, sum = 1;

	for(i=2; i*i<n; i++){
		if(!(n%i))
			sum += i+n/i;
	}
	if(i*i == n)
		sum += i;
	return sum;
}

void find(int n, int sum)
{
	if(n == 1)
		printf("%5d  DEFICIENT\n", n);
	else if(n>sum)
		printf("%5d  DEFICIENT\n", n);
	else if(n<sum)
		printf("%5d  ABUNDANT\n", n);
	else
		printf("%5d  PERFECT\n", n);
}

int main(void)
{
	int n, sum;

	printf("PERFECTION OUTPUT\n");
	while(scanf("%d", &n) != EOF){
		if(!n){
			printf("END OF OUTPUT\n");
			exit(0);
		}
		sum = judge(n);
		find(n, sum);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
