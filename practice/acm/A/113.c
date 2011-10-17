/*###########################################################################*/
/*       title: Power of Cryptography                                        */
/*                                                                           */
/*           start: */
/*                                                                           */
/*                finish: */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   113   C     */
/*                                                                           */
/*###########################################################################*/
#include <stdio.h>
#include <math.h>
                                                                                
int main()
{
	double p,k;
	long n,i;

	while(scanf("%ld %lf",&n,&p)!=EOF) {
		printf("%.0lf\n",exp(log(p)/n));
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
