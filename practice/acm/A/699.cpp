/* @JUDGE_ID:	10319NX	699 C++*/
#include<stdio.h>
#include<string.h>

int num[1000];
int min, max;

void search(int val, int pos)
{
	int n;

	if(pos < min)
		min = pos;
	if(pos > max)
		max = pos;
	num[pos] += val;
	scanf("%d", &n);
	if(n != -1)
		search(n, pos-1);
	scanf("%d", &n);
	if(n != -1)
		search(n, pos+1);
}

int main()
{
	int val;
	int i;
	int round=0;

	while(scanf("%d", &val) != EOF){
		if(val == -1)
			break;
		if(round)
			printf("\n");
		memset(num, 0, sizeof(num));
		min = 500;
		max = 500;
		search(val, 500);
		printf("Case %d:\n", round+1);
		for(i=min; i<=max; i++)
			printf("%d ", num[i]);
		printf("\n");
		round++;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
