/*###########################################################################*/
/*       title:                        Combinations                          */
/*                                                                           */
/*           start: 2001年  6月30日 周六 14時02分21秒 CST                    */
/*                                                                           */
/*                finish: */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   369   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>

long long int prime[40], lim=1;
long long int tab[40];
long long int n, m;

long long int nice(long long int n)
{
	long long int i;

	for(i=0; i<lim; i++)
		if(!(n%prime[i]))
			return 0;
	return 1;
}

void make_prime(long long int n)
{
	long long int i;

	prime[0] = 2;
	for(i=3; i<100; i+=2)
		if(nice(i)){
			prime[lim] = i;
			lim++;
		}
}

void count(long long int n, long long int a)
{
	long long int i=0;

	while(n!=1 && prime[i]<=n){
		if(!(n%prime[i])){
			n /= prime[i];
			tab[i] += a;
		}
		else
			i++;
	}
}

void output(void)
{
	long long int ans=1, i, j;

	for(i=0; i<lim; i++){
		for(j=0; j<tab[i]; j++)
			ans *= prime[i];
	} 
	for(i=0; i<lim; i++){
		for(j=0; j<tab[i]*-1; j++)
			ans /= prime[i];
	} 
	printf("%lld things taken %lld at a time is %lld exactly.\n", n, m, ans);
}

int main(void)
{
	long long int max, min, i;

	make_prime(100);
	while(scanf("%lld%lld", &n, &m) == 2){
		if(n==0 && m==0)
			exit(0);
		memset(tab, 0, sizeof(tab));
		max = (n-m>m) ? n-m : m;
		min = (n-m<m) ? n-m : m;
		for(i=max+1; i<=n; i++)
			count(i, 1);
		for(i=1; i<=min; i++)
			count(i, -1);
		output();
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
