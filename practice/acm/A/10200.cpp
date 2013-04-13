/* @JUDGE_ID:   10319NX 10200 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>

int prime[1400];
int lim;
int win[10002];

int is_prime(int n)
{
	int i;

	for(i=0; i<lim&&prime[i]*prime[i]<=n; i++)
		if(!(n%prime[i]))
			return 0;
	return 1;
}

void make_tab(void)
{
	int i, n=11000;

	lim = 1;
	prime[0] = 2;
	for(i=3; i<=n; i+=2){
		if(is_prime(i))
			prime[lim++] = i;
	}
}

void make_win(void)
{
	int i;

	win[0] = 0;
	for(i=0; i<=10000; i++){
		if(is_prime(i*i+i+41))
			win[i+1] = win[i]+1;
		else
			win[i+1] = win[i];
	}
}

int main(void)
{
	int from, to;

	make_tab();
	make_win();
	while(scanf("%d%d", &from, &to) != EOF){
		to++;
		printf("%.2lf\n", 100.0*(win[to]-win[from])/(to-from));
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
