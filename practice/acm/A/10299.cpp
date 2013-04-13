#include<stdio.h>

int prime[3401];
int lp;

void initial()
{
	int i, j;
	bool flag;

	prime[0] = 2;
	lp = 1;
	for(i=3; i*i<=1000000000; i+=2){
		flag = true;
		for(j=0; j<lp && prime[j]*prime[j]<=i; j++){
			if(!(i%prime[j])){
				flag = false;
				break;
			}
		}
		if(flag)
			prime[lp++] = i;
	}
}

int main()
{
	int n;
	double sum;
	int i;

	initial();
	while(scanf("%d", &n) != EOF && n){
		if(n == 1){
			printf("0\n");
			continue;
		}
		sum = n;
		for(i=0; i<lp && prime[i]<=n; i++){
			if(!(n%prime[i])){
				sum *= (1-1.0/prime[i]);
				while(!(n%prime[i]))
					n /= prime[i];
			}
		}
		if(n != 1)
			sum *= (1-1.0/n);
		printf("%.0lf\n", sum);
	}
	return 0;
}
