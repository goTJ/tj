//### PROGRAM
/*
ID: flyherm003
PROG: agrinet
*/
#include<stdio.h>
#define N 106

int way[N][N];
int used[N], min[N];
int lim, num;

void initRoot(int n)
{
	int i;

	for(i=0; i<lim; i++){
		min[i] = way[n][i];
		used[i] = 0;
	}
	used[n] = 1;
	num = 1;
}

int extend(void)
{
	int i;
	int node=-1;

	for(i=0; i<lim; i++){
		if(!used[i] && (node==-1 || min[i]<min[node]))
			node = i;
	}
	if(node == -1)
		printf("Error!!\n");
	return node;
}

void change(int node)
{
	int i;

	used[node] = 1;
	num++;
	for(i=0; i<lim; i++)
		min[i] = (way[node][i] < min[i]) ? way[node][i] : min[i];
}

int main(void)
{
	int i, j;
	int node, sum;
/**/	freopen("agrinet.in", "r", stdin);
/**/	freopen("agrinet.out", "w", stdout);

	while(scanf("%d", &lim) != EOF){
		for(i=0; i<lim; i++)
			for(j=0; j<lim; j++)
				scanf("%d", &way[i][j]);
		initRoot(0);
		sum = 0;
		while(num < lim){
			node = extend();
			sum += min[node];
			change(node);
		}
		printf("%d\n", sum);
	}
	return 0;
}
//### END