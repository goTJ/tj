//### PROGRAM
/*
ID: flyherm003
PROG: crypt1
*/
#include<stdio.h>
#include<string.h>
#define DEPTH 5

int canUse[10];
int sum;

int illegal(int n)
{
	int t;

	while(n){
		t = n%10;
		if(!canUse[t])
			return 0;
		n /= 10;
	}
	return 1;
}

int check(int way[])
{
	int up=0;
	int tmp;
	int i;

	for(i=0; i<3; i++)
		up = up*10+way[i];
	for(i=3; i<5; i++){
		tmp = up*way[i];
		if(!illegal(tmp) || tmp/1000)
			return 0;
	}
	tmp = up*way[3]*10+up*way[4];
	if(!illegal(tmp) || tmp/10000)
		return 0;
	return 1;
}

void search(int depth, int way[])
{
	int i;

	if(depth == DEPTH){
		if(check(way))
			sum++;
	}
	else{
		for(i=1; i<=9; i++)
			if(canUse[i]){
				way[depth] = i;
				search(depth+1, way);
			}
	}
}

int main(void)
{
	int n, i, t;
	int way[DEPTH];
////	freopen("crypt1.in", "r", stdin);
////	freopen("crypt1.out", "w", stdout);

	while(scanf("%d", &n) != EOF){
		memset(canUse, 0, sizeof(canUse));
		sum = 0;
		for(i=0; i<n; i++){
			scanf("%d", &t);
			canUse[t] = 1;
		}
		search(0, way);
		printf("%d\n", sum);
	}
	return 0;
}
//### END
