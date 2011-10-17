/* @JUDGE_ID:   10319NX 10257 C "­·Ä_¨©^^" */
#include<stdio.h>

int main(void)
{
	int s, p, y, j;
	int t, n;
	int sum;

	while(scanf("%d%d%d%d", &s, &p, &y, &j) != EOF){
		sum = j+12;
		t = sum-p-y;
		if(t%3 == 2){
			y++;
			p++;
		}
		else if(t%3==1 && !(y-s-p))
			y++;
		else if(t%3 == 1)
			p++;
		n = t/3;
		printf("%d %d %d\n", y+n, p+n, n);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
