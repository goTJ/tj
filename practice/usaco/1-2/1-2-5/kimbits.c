//### PROGRAM
/*
ID: flyherm003
PROG: kimbits
*/
#include<stdio.h>

long long int num[33][33];

long long int c(long long int a, long long int b)
{
	long long int i;
	long long int sum=1;

	b = (b < a-b) ? b : a-b;
	for(i=1; i<=b; i++,a--){
		sum *= a;
		sum /= i;
	}
	return sum;
}

void make_tab(void)
{
	long long int i, j;

	for(i=0; i<=32; i++){
		for(j=0; j<=i; j++){
			num[i][j] = c(i, j);
			if(j > 0)
				num[i][j] += num[i][j-1];
		}
	}
}

int main(void)
{
	int total, n;
	long long int rank;
	int i;
////	freopen("kimbits.in", "r", stdin);
////	freopen("kimbits.out", "w", stdout);

	make_tab();
	while(scanf("%d%d%lld", &total, &n, &rank) != EOF){
		for(i=total; i>0; i--){
			if(n > i-1)
				n = i-1;
			if(rank > num[i-1][n]){
				printf("1");
				rank -= num[i-1][n];
				n--;
			}
			else
				printf("0");
		}
		printf("\n");
	}
	return 0;
}
//### END
