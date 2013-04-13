/* @JUDGE_ID:   10319NX 294 C */
#include<stdio.h>

int compute(int val)
{
	int i;
	int sum=0;

	for(i=1; i*i<=val; i++){
		if(!(val%i)){
			if(i*i == val)
				sum += 1;
			else
				sum += 2;
		}
	}
	return sum;
}

int main(void)
{
	int from, to, i;
	int times, round;
	int max, num, t;

	scanf("%d", &round);
	for(times=0; times<round; times++){
		scanf("%d %d", &from, &to);
		max = 0;
		for(i=from; i<=to; i++){
			if((t=compute(i)) > max){
				max = t;
				num = i;
			}
		}
		printf("Between %d and %d, %d has a maximum of %d divisors.\n"
				, from, to, num, max);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
