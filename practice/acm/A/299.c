/* @JUDGE_ID:   10319NX 299 C */
#include<stdio.h>
#include<stdlib.h>

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int main(void)
{
	int round, len;
	int i, j, k;
	int s[55], count;

	scanf("%d", &round);
	for(i=0; i<round; i++){
		count = 0;
		scanf("%d", &len);
		for(j=0; j<len; j++)
			scanf("%d", &s[j]);
		for(j=0; j<len; j++)
			for(k=0; k<len-1; k++)
				if(s[k] > s[k+1]){
					swap(&s[k], &s[k+1]);
					count++;
				}
		printf("Optimal train swapping takes %d swaps.\n", count);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
