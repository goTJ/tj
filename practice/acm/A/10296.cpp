#include<stdio.h>
#define INF 100000000
#define MAX 16

int map[MAX][MAX];
int N;
int single[MAX], ns;
bool used[MAX];
int min;

void gogogo(int left, int cost)
{
	if(cost >= min)
		return;
	if(!left){
		if(cost < min)
			min = cost;
		return;
	}
	int i, j;
	for(i=0; i<ns; i++){
		if(!used[i]){
			for(j=i+1; j<ns; j++){
				if(!used[j]){
					used[i] = used[j] = true;
					gogogo(left-2, cost+map[single[i]][single[j]]);
					used[i] = used[j] = false;
				}
			}
			break;
		}
	}
	return;
}
void floyd(int map[][MAX], int N)
{
	int i, j, k;

	for(k=0; k<N; k++){
		for(i=0; i<N; i++){
			for(j=0; j<N; j++){
				if(map[i][k]+map[k][j] < map[i][j])
					map[i][j] = map[i][k]+map[k][j];
			}
		}
	}
}

int main(void)
{
	int i, j;
	int M;
	int a, b, d;
	int sum;
	int link[MAX];

	while(scanf("%d", &N) != EOF && N){
		scanf("%d", &M);
		for(i=0; i<N; i++){
			for(j=0; j<N; j++)
				map[i][j] = INF;
		}
		for(i=0; i<N; i++)
			link[i] = map[i][i] = 0;
		sum = 0;
		for(i=0; i<M; i++){
			scanf("%d%d%d", &a, &b, &d);
			a--, b--;
			if(d < map[a][b])
				map[a][b] = map[b][a] = d;
			link[a]++, link[b]++;
			sum += d;
		}
		floyd(map, N);
		for(i=0,ns=0; i<N; i++){
			if(link[i]%2){
				single[ns] = i;
				used[ns++] = false;
			}
		}
		min = INF;
		gogogo(ns, 0);
		printf("%d\n", min+sum);
	}
	return 0;
}
