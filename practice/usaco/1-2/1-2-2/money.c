//### PROGRAM
/*
ID: flyherm003
PROG: money
*/
#include<stdio.h>
#include<string.h>

int val[10002];
int num;

void find(int money)
{
	int i;

	for(i=money; i<=num; i++)
		val[i] += val[i-money];
}

int main(void)
{
	int i, lim, money;
	freopen("money.in",  "r",  stdin);
	freopen("money.out",  "w",  stdout);

	while(scanf("%d%d", &lim, &num) != EOF){
		memset(val, 0, sizeof(val));
		val[0] = 1;
		for(i=0; i<lim; i++){
			scanf("%d", &money);
			find(money);
		}
		printf("%d\n", val[num]);
	}
	return 0;
}
//### END