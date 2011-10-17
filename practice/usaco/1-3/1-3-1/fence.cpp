//### PROGRAM
/*
ID: flyherm003
PROG: fence
*/
#include<stdio.h>
#include<string.h>

int way[505][505];
int degree[505];

void display(void)
{
	int i, j;

	printf("   ");
	for(i=1; i<=10; i++)
		printf("%02d ", i);
	printf("\n");
	for(i=1; i<=10; i++){
		printf("%02d ", i);
		for(j=1; j<=10; j++)
			printf("%2d ", way[i][j]);
		printf("\n");
	}
}

void initial(void)
{
	memset(way, 0, sizeof(way));
	memset(degree, 0, sizeof(degree));
}

void Eule(int node, int ans[], int *pos)
{
//	printf("go eule: %d\n", node);
//	display();
	if(degree[node]){
		for(int i=1; i<=500; i++){
			if(way[node][i]){
				way[node][i]--;
				way[i][node]--;
				degree[node]--;
				degree[i]--;
				Eule(i, ans, pos);
			}
		}
	}
	ans[(*pos)++] = node;
}

int main(void)
{
	int n, i;
	int a, b;
	int min;
	int ans[1200], pos;
/**/	freopen("fence.in", "r", stdin);
/**/	freopen("fence.out", "w", stdout);

	while(scanf("%d", &n) != EOF){
//		printf("start: %d\n", n);
		initial();
		min = 505;
		pos = 0;
		for(i=0; i<n; i++){
			scanf("%d%d", &a, &b);
			min = (a<min) ? a : min;
			min = (b<min) ? b : min;
			way[a][b]++;
			way[b][a]++;
			degree[a]++;
			degree[b]++;
		}
//		display();
		for(i=1; i<=500; i++)
			if(degree[i]%2)
				break;
		if(i == 501)
			i = min;
		Eule(i, ans, &pos);
//		printf("find OK\n");
		for(i=pos-1; i>=0; i--)
			printf("%d\n", ans[i]);
	}
	return 0;
}
//### END