/* @JUDGE_ID:   10319NX 495 C */
#include<stdio.h>
#include<string.h>

enum { N = 100000 };

struct fly{
	int len;
	int num[300];
}fib[5003];

int add(int a[], int la, int b[], int lb)
{
	int i, max;

	for(i=0; i<lb; i++)
		a[i] += b[i];
	max = (la > lb) ? la : lb;
	for(i=0; i<max; i++){
		while(a[i] >= N){
			a[i+1] += a[i]/N;
			a[i] %= N;
		}
	}
	if(a[i] > 0)
		return max+1;
	else
		return max;
}

void initial(void)
{
	int a[400], b[400], tmp[400];
	int len_a, len_b, len_t;
	int num, i;
	int j;

	fib[2].num[0] = fib[1].num[0] = fib[2].len = fib[1].len = 1;
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	a[0] = 1;
	b[0] = 1;
	len_a = len_b = 1;
	for(i=3; i<=5000; i++){
		len_t = len_a;
		memmove(tmp, a, sizeof(a[0])*len_a);
		len_a = add(a, len_a, b, len_b);
		len_b = len_t;
		memmove(b, tmp, sizeof(tmp[0])*len_t);
		memmove(fib[i].num, a, sizeof(a[0])*len_a);
		fib[i].len = len_a;
	}
}

int main(void)
{
	int n, i;

	initial();
	while(scanf("%d", &n) != EOF){
printf("The Fibonacci number for %d is %d", n, fib[n].num[fib[n].len-1]);
		for(i=fib[n].len-2; i>=0; i--)
			printf("%0*d", 5, fib[n].num[i]);
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
