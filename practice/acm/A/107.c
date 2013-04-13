/* @JUDGE_ID:   10319NX 107 C */
#include<stdio.h>
#include<math.h>

int check(int n, int i)
{
	int sum=0;

	while(!(n%i)){
		n /= i;
		sum++;
	}
	if(n == 1)
		return sum;
	else
		return 0;
}

int output(int n, int r)
{
	int num=0;
	int p=1, q=1;
	int i;

	for(i=0; i<n; i++){
		num += p;
		p *= r-1;
	}
	printf("%d ", num);
	num = 0;
	for(i=0; i<=n; i++,p/=(r-1),q*=r)
		num += p*q;
	printf("%d\n", num);
}

int main(void)
{
	int a, b;
	int i;
	int n;

	while(scanf("%d%d", &a, &b) != EOF){
		if(!a && !b)
			break;
		if(a==1 && b==1)
			printf("0 1\n");
		for(i=2; i<=a; i++){
			if((n=check(a, i)) && pow(i-1, n)==b){
				output(n, i);
				break;
			}
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
