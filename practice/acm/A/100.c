/*   @JUDGE_ID:   10387ZJ   100   C*/ 
#include<stdio.h>
#define MAX(a, b) ((a>b) ? a:b)
#define MIN(a, b) ((a<b) ? a:b)

int judge(int n)
{
	int many = 1;

	while(n != 1){
		many++;
		if(n%2)
			n = 3*n+1;
		else
			n /= 2;
	}
	return many;
}

int main(void)
{
	int m, n, min, max, i, many, t;

	while(scanf("%d%d", &n, &m) != EOF){
		many = 0;
		min = MIN(n, m);
		max = MAX(n, m);
		for(i=min; i<=max; i++){
			if((t=judge(i)) > many)
				many = t;
		}
		printf("%d %d %d\n", n, m, many);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
