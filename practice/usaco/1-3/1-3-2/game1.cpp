//### PROGRAM
/*
ID: flyherm003
PROG: game1
*/
#include<stdio.h>
#define MAX 100

int best[MAX][MAX];	// best[first][end]
int val[MAX];
int lim;

int max(int a, int b)
{
	return (a>b) ? a : b;
}

int sum(int first, int end)
{
	int tmp;

	for(tmp=0; first<=end; first++)
		tmp += val[first];
	return tmp;
}

int main(void)
{
	int i, k;
////	freopen("game1.in", "r", stdin);
////	freopen("game1.out", "w", stdout);

	while(scanf("%d", &lim) != EOF){
		for(i=0; i<lim; i++)
			scanf("%d", &val[i]);
		for(k=0; k<lim; k++){
			for(i=0; i+k<lim; i++){
				best[i][i+k] = max(val[i]+sum(i+1, i+k)-best[i+1][i+k], val[i+k]+sum(i, i+k-1)-best[i][i+k-1]);
			}
		}
		printf("%d %d\n", best[0][lim-1], sum(0, lim-1)-best[0][lim-1]);
	}
	return 0;
}
//### END
