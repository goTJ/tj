/*   @JUDGE_ID:   10319NX   10055   C  */
#include<stdio.h>

double min_n(double a, double b)
{
	return ((a<b) ? a:b);
}

double max_n(double a, double b)
{
	return ((a>b) ? a:b);
}
int main(void)
{
	double a, b, min, max;

	while(scanf("%lf%lf", &a, &b) != EOF){
		min = min_n(a, b);
		max = max_n(a, b);
		printf("%.0lf\n", max-min);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */ 
