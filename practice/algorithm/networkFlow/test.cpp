#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define N 300
#define M 1000

int main()
{
	int n;
	int i;
	int a, b, c;
	int s, t;

	srand(time(NULL));
	printf("%d\n", N);
	n = (int)((double)rand()/RAND_MAX*M);
	printf("%d\n", n);
	for(i=0; i<n; i++){
		a = (int)((double)rand()/RAND_MAX*N)+1;
		b = (int)((double)rand()/RAND_MAX*N)+1;
		c = (int)((double)rand()/RAND_MAX*100)+1;
		printf("%d %d %d\n", a, b, c);
	}
	do{
		a = (int)((double)rand()/RAND_MAX*N)+1;
		b = (int)((double)rand()/RAND_MAX*N)+1;
	}while(a == b);
	printf("%d %d\n", a, b);
	return 0;
}
