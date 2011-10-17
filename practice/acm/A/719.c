/* @JUDGE_ID:   10319NX 719 C */
#include<stdio.h>
#include<string.h>
#include<limits.h>

char bead[100005];
int len;
int num[100005];

int c(int n)
{
	return (n+len)%len;
}

int main(void)
{
	int round, times;
	int i, j;
	int min, pos=0, flag;

	scanf("%d", &round);
	for(times=0; times<round; times++){
		scanf("%s", bead);
		min = 0;
		memset(num, 0, sizeof(num));
		len = strlen(bead);
		for(i=0; i<len; i++){
			for(j=0; j<len; j++){
				if(num[j] == min)
					num[j] = bead[c(j+i)];
				else
					num[j] = INT_MAX;
			}
			min = num[0];
			pos = 0;
			flag = 1;
			for(j=1; j<len; j++){
				if(num[j] == min)
					flag = 0;
				else if(num[j] < min){
					min = num[j];
					pos = j;
					flag = 1;
				}
			}
			if(flag)
				break;
		}
		printf("%d\n", pos+1);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
