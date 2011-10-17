/* @JUDGE_ID:   10319NX 615 C */
#include<stdio.h>
#include<string.h>

enum { MAX=1000 };
int num[MAX];

int check(void)
{
	int i;
	int flag = 1;
	int none=1;

	for(i=0; i<MAX; i++){
		if(num[i])
			none = 0;
		if(num[i]==1){
			if(flag)
				flag = 0;
			else
				return 0;
		}
	}
	if(flag && !none)
		return 0;
	return 1;
}

int main(void)
{
	int times=1;
	int a, b, flag=1;

	memset(num, 0, sizeof(num));
	while(scanf("%d%d", &a, &b) != EOF){
		if(a==-1 && b==-1)
			break;
		else if(!a && !b){
			if(flag && check())
				printf("Case %d is a tree.\n", times);
			else
				printf("Case %d is not a tree.\n", times);
			times++;
			flag = 1;
			memset(num, 0, sizeof(num));
		}
		else{
			num[a] = num[a]|1;
			if(num[b]&2)
				flag = 0;
			else
				num[b] = num[b]|2;
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
