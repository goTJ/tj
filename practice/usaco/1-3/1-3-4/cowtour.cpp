//### PROGRAM
/*
ID: flyherm003
PROG: cowtour
*/
#include<stdio.h>
#include<math.h>
#define MAX 150

struct Coor{
	int x;
	int y;
};

int lim;
Coor pos[MAX];
double dis[MAX][MAX];
double max[MAX];

inline bool equal(double a, double b)
{
	if(a>b-0.00001 && a<b+0.00001)
		return true;
	return false;
}

inline bool great(double a, double b)
{
	if(a > b-0.00001)
		return true;
	return false;
}

inline bool little(double a, double b)
{
	if(a < b+0.00001)
		return true;
	return false;
}

void floyd(void)
{
	int i, j, k;

	for(k=0; k<lim; k++){
		for(i=0; i<lim; i++){
			for(j=0; j<lim; j++){
				if(!equal(dis[i][k], 0.0) && !equal(dis[k][j], 0.0) && (little(dis[i][k]+dis[k][j], dis[i][j]) || equal(dis[i][j], 0.0)))
					dis[i][j] = dis[i][k]+dis[k][j];
			}
		}
	}
}

int main(void)
{
	double min=0.0, lower=0.0;
	freopen("cowtour.in",  "r",  stdin);
	freopen("cowtour.out",  "w",  stdout);

	scanf("%d", &lim);
	for(int i=0; i<lim; i++)
		scanf("%d%d\n", &pos[i].x, &pos[i].y);
	for(int i=0; i<lim; i++){
		char c;
		for(int j=0; j<lim; j++){
			scanf("%c", &c);
			if(c == '1'){
				int tx=pos[i].x-pos[j].x;
				int ty=pos[i].y-pos[j].y;
				dis[i][j] = dis[j][i] = sqrt(tx*tx+ty*ty);
			}else
				dis[i][j] = dis[j][i] = 0.0;
		}
		scanf("%c", &c);
	}
	floyd();
	for(int i=0; i<lim; i++){
		max[i] = dis[i][0];
		for(int j=1; j<lim; j++){
			if(i!=j && great(dis[i][j], max[i]))
				max[i] = dis[i][j];
		}
		if(equal(lower, 0.0) || great(max[i], lower))
			lower = max[i];
	}
	for(int i=0; i<lim; i++){
		for(int j=i+1; j<lim; j++){
			if(equal(dis[i][j], 0.0)){
				int tx=pos[i].x-pos[j].x;
				int ty=pos[i].y-pos[j].y;
				double d=sqrt(tx*tx+ty*ty)+max[i]+max[j];
				if(equal(min, 0.0) || little(d, min))
					min = d;
			}
		}
	}
	if(little(min, lower))
		min = lower;
	printf("%.6lf\n", min);
	return 0;
}
//### END
