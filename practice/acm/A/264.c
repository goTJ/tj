/*###########################################################################*/
/*       title: Count                                                        */
/*                                                                           */
/*           start: 2001�~  7��12�� �P�| 18��03��05�� CST                    */
/*                                                                           */
/*                finish: 2001�~  7��12�� �P�| 18��27��38�� CST              */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   264   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>

unsigned int times;
unsigned int flag;
unsigned int m;

int main(void)
{
	unsigned int n, i;

	while(scanf("%u", &n) == 1){
		printf("TERM %u IS ", n);
		times = 1;
		for(; n>times; times++)
			n -= times;
		flag = times%2;
		times++;
		m = times-n;
		if(flag)
			printf("%u/%u\n", m, n);
		else
			printf("%u/%u\n", n, m);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
