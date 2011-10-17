/* @JUDGE_ID:   10319NX 10219 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>
#include<math.h>

int main(void)
{
	double M, N;
	double sum;
	double i;

	while(scanf("%lf%lf", &M, &N) != EOF){
		sum = 0.0;
		for(i=0.0; i<N; i++){
			sum += log10(M-i);
			sum -= log10(i+1);
		}
		printf("%.0lf\n", sum+0.500001);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
