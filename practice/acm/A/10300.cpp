/* @JUDGE_ID:   10319NX 10300 C++ */
#include<stdio.h>

int main(void)
{
	int times, round;
	int num, i;
	long long int space, animal, degree;
	long long int sum;

	scanf("%d", &times);
	for(round=0; round<times; round++){
		sum = 0;
		scanf("%d", &num);
		for(i=0; i<num; i++){
			scanf("%lld%lld%lld", &space, &animal, &degree);
			sum += space*degree;
		}
		printf("%lld\n", sum);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
