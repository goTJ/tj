/*   @JUDGE_ID:   10318MP   591   C*/ 
#include<stdio.h>

int middle(char *s, int n)
{
	int i, m=0;

	for(i=0; i<n; i++)
		m += s[i];
	return m/n;
}

int main(void)
{
	char s[50];
	int time=1, n, i, mid, sum;

	while(scanf("%d", &n) != EOF){
		if(!n)
			exit(0);
		sum = 0;
		for(i=0; i<n; i++)
			scanf("%d", s+i);
		mid = middle(s, n);
		for(i=0; i<n; i++)
			if(s[i]<mid)
				sum += mid-s[i];
		printf("Set #%d\n", time);
		printf("The minimum number of moves is %d.\n", sum);
		time++;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
