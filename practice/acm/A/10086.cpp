#include<stdio.h>

#define N 301

struct Tab{
	int cost;
	int from;
};

void output(const Tab tab[][N], int pos, int depth)
{
	if(depth){
		output(tab, tab[depth][pos].from, depth-1);
		if(depth > 1)
			printf(" ");
		printf("%d", pos-tab[depth][pos].from);
	}
}

int main()
{
	Tab tab[31][N];
	int max;
	int buf[21];
	int n, m;
	int sum;
	int i, j, k;
	int temp;

	while(scanf("%d%d", &max, &temp) != EOF && (max || temp)){
		scanf("%d", &n);
		sum = 0;
		tab[0][0].cost = 0;
		for(i=1; i<=n; i++){
			// input
			scanf("%d", &m);
			buf[0] = 0;
			for(j=1; j<=m; j++)
				scanf("%d", &buf[j]);
			for(j=1; j<=m; j++){
				scanf("%d", &temp);
				buf[m-j] += temp;
			}
			// initial
			for(j=0; j<=sum&&j<=max; j++){
				tab[i][j].cost = tab[i-1][j].cost+buf[0];
				tab[i][j].from = j;
			}
			for(j=1; j<=m; j++){
				tab[i][j+sum].cost = tab[i-1][sum].cost+buf[j];
				tab[i][j+sum].from = sum;
			}
			// judge
			for(j=0; j<sum; j++){
				for(k=1; k<=m&&j+k<=max; k++){
					if(tab[i-1][j].cost+buf[k] < tab[i][j+k].cost){
						tab[i][j+k].cost = tab[i-1][j].cost+buf[k];
						tab[i][j+k].from = j;
					}
				}
			}
			sum += m;
		}
		printf("%d\n", tab[n][max].cost);
		output(tab, max, n);
		printf("\n\n");
	}
	return 0;
}
