/*###########################################################################*/
/*       title: Goldbach's                                                   */
/*                                                                           */
/*           start: 2001年  7月14日 周六 17時03分23秒 CST                    */
/*                                                                           */
/*                finish: 2001年  7月16日 周一 07時06分59秒 CST              */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   543   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>

unsigned int prime[1000000];
unsigned int pri_lim=1;
unsigned int num;

int isprime(unsigned int n)
{
	int i;

	for(i=0; i<pri_lim && prime[i]*prime[i]<=n; i++)
		if(!(n%prime[i]))
			return 0;
	return 1;
}

int main(void)
{
	unsigned int i, tmp, flag=1;

	prime[0] = 2;
	for(i=3; i*i<=1000000; i+=2)
		if(isprime(i)){
			prime[pri_lim++] = i;
		}
	while(scanf("%u", &num) == 1){
		flag = 1;
		if(!num)
			exit(0);
		for(i=1; prime[i]<=num/2; i++){
			if(isprime(num-prime[i])){
			  printf("%u = %u + %u\n", num, prime[i], num-prime[i]);
				flag = 0;
				break;
			}
		}
		if(flag)
			printf("Goldbach's conjecture is wrong.\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
