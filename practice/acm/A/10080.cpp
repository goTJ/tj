/* @JUDGE_ID:   10319NX 10080 C++ */
#include<stdio.h>
#include<string.h>
#include<math.h>
#define MAX 250

typedef struct coor{
	double x, y;
}coor;
int queue[MAX];
int head, tail;

int capacity[MAX][MAX];
int n, m, lim;

void display(void)
{
	int i, j;

	for(i=0; i<lim; i++){
		for(j=0; j<lim; j++)
			printf("%d ", capacity[i][j]);
		printf("\n");
	}
}

int reachHole(coor a, coor b, double dis)
{
	double d=(a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);

	return (d<dis || fabs(dis-d)<0.000001);
}

void enQueue(int n)
{
	tail = (tail+1)%MAX;
	queue[tail] = n;
}

int deQueue(void)
{
	head = (head+1)%MAX;
	return queue[head];
}

int bfs(int s, int t, int way[])
{
	int used[MAX];
	int pos, i;

//	printf("go bfs\n");
	memset(used, 0, sizeof(used));
	head = -1;
	tail = -1;
	enQueue(s);
	while(head != tail){
//		printf("%d %d\n", head, tail);
		pos = deQueue();
//		printf("pos: %d\n", pos);
		for(i=0; i<lim; i++){
			if(!used[i] && capacity[pos][i] > 0){
				way[i] = pos;
				used[i] = 1;
				if(i == t)
					return 1;
				enQueue(i);
			}
		}
	}
	return 0;
}

int main(void)
{
	int i, j;
	int s, v, d;
	int way[MAX];
	coor gopher[MAX], hole[MAX];
	int num;

	while(scanf("%d%d%d%d", &n, &m, &s, &v) != EOF){
		d = s*s*v*v;
		for(i=1; i<=n; i++)
			scanf("%lf%lf", &gopher[i].x, &gopher[i].y);
		for(i=1; i<=m; i++)
			scanf("%lf%lf", &hole[i].x, &hole[i].y);
		memset(capacity, 0, sizeof(capacity));
		for(i=1; i<=n; i++){
			for(j=1; j<=m; j++)
				if(reachHole(gopher[i], hole[j], d))
					capacity[i][j+n] = 1;
		}
		for(i=1; i<=n; i++)
			capacity[0][i] = 1;
		for(i=1; i<=m; i++)
			capacity[n+i][n+m+1] = 1;
		lim = n+m+2;
		num = 0;
//		display();
		while(bfs(0, lim-1, way)){
//			printf("find\n");
			num++;
			for(i=n+m+1; i!=0; i=way[i]){
				capacity[way[i]][i] -= 1;
				capacity[i][way[i]] += 1;
			}
		}
		printf("%d\n", n-num);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */