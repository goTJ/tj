#include<stdio.h>
#define N 103

int map[N+2][N+2];
int lim;

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void initial(void)
{
	int n, i, j, k;
	int point[N*2]={0};
	int len[N];
	int index;
	int lt[2];
	int tab[2][8];

	lim = 1;
	scanf("%d", &n);
	for(i=0; i<n; i++){
		scanf("%d", &index);
		scanf("%d", &len[index]);
		scanf("%d%d", &lt[0], &lt[1]);
		for(j=0; j<2; j++){
			for(k=0; k<lt[j]; k++)
				scanf("%d", &tab[j][k]);
		}
		for(j=0; j<2; j++){
			if(point[index+n*j]){
				if(!(point[index+n*j]==point[tab[j][0]] || point[index+n*j]==point[tab[j][0]+n]))
					swap(&point[index], &point[index+n]);
			}
			if(!point[index+n*j]){
				point[index+n*j] = lim;
				for(k=0; k<lt[j]; k++){
					if(!point[tab[j][k]])
						point[tab[j][k]] = lim;
					else
						point[tab[j][k]+n] = lim;
				}
				lim++;
			}
		}
	}
	for(i=1; i<=n; i++){
		map[point[i]][point[i+n]] = len[i];
		map[point[i+n]][point[i]] = len[i];
	}
	/*
	for(j=1; j<=n; j++)
		printf("%2d: %2d\t%2d: %2d\n", j, point[j], j+n, point[j+n]);
	printf("\n");
	for(i=1; i<lim; i++){
		for(j=1; j<lim; j++)
			printf("%3d ", map[i][j]);
		printf("\n");
	}
	printf("\n");
	*/
}

int dijkstra(int s, int t)
{
	int dis[N+2];
	int i;
	int mini=-1;
	bool used[N+2]={0};

//	printf("go dijkstra: %d %d\n", s, t);
	for(i=1; i<lim; i++)
		dis[i] = map[s][i];
	used[s] = true;
	while(mini!=s && mini!=t){
		mini = s;
		/*
		for(i=1; i<lim; i++)
			printf("%2d ", dis[i]);
		printf("\n");
		*/
		for(i=1; i<lim; i++){
			if(!used[i] && dis[i] && (mini==s || dis[i]<dis[mini]))
				mini = i;
		}
//		printf("%d: %d\n", mini, dis[mini]);
		if(mini != s){
			used[mini] = true;
			for(i=1; i<lim; i++){
				if(!used[i] && map[mini][i] && (!dis[i] || dis[mini]+map[mini][i]<dis[i]))
					dis[i] = dis[mini]+map[mini][i];
			}
		}
//		printf("\n");
	}
//	printf("dis: %d\n", dis[t]);
	return dis[t];
}

int main(void)
{
	int i, j;
	int t, tmp;
	int min=26000;

	initial();
	for(i=1; i<lim; i++){
		for(j=i+1; j<lim; j++){
			if(map[i][j]){
				t = map[i][j];
				map[i][j] = map[j][i] = 0;
				tmp = dijkstra(i, j);
				if(tmp && tmp+t < min)
					min = tmp+t;
				map[i][j] = map[j][i] = t;
			}
		}
	}
	printf("%d\n", min);
	return 0;
}
