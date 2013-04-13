#include<stdio.h>
#include<string.h>
#include<stdlib.h>

enum { N = 1000 };

int mul(int a[], int len, int n)
{
	int i;

	for(i=0; i<len; i++)
		a[i] *= n;
	for(i=0; i<len; i++){
		if(a[i] >= N){
			a[i+1] += a[i]/N;
			a[i] %= N;
		}
	}
	if(a[i] > 0)
		return len+1;
	return len;
}

int main(void)
{
	int a[300], count[10];
	int n, i, j, len;

	while(scanf("%d", &n) != EOF){
		if(!n)
			exit(0);
		memset(a, 0, sizeof(a));
		memset(count, 0, sizeof(count));
		a[0] = len = 1;
		for(i=2; i<=n; i++)
			len = mul(a, len, i);
		for(i=0; i<len-1; i++)
			for(j=0; j<3; j++){
				count[a[i]%10]++;
				a[i] /= 10;
			}
		while(a[len-1] > 0){
			count[a[len-1]%10]++;
			a[len-1] /= 10;
		}
		printf("%d! --\n", n);
		for(i=0; i<10; i++){
			printf("\t(%d)\t%d", i, count[i]);
			if(i%5 == 4)
				printf("\n");
		}
	}
	return 0;
}
