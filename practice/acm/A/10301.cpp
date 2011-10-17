/* @JUDGE_ID:   10318MP 10301 C++ "附中工研社第23屆" */
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

typedef struct ring{
	double coorX;
	double coorY;
	double radius;
}Ring;
int reach[100][100];
int component[100];

Ring ring[100];
int num;

void initial(void)
{
	int i;

	for(i=0; i<num; i++){
		component[i] = 0;
		reach[i][i] = 0;
	}
}

void input(void)
{
	int i;

	for(i=0; i<num; i++)
		scanf("%lf%lf%lf", &ring[i].coorX, &ring[i].coorY, &ring[i].radius);
}

int is_reach(int a, int b)
{
	double cLine, tx, ty;

	tx = ring[a].coorX-ring[b].coorX;
	ty = ring[a].coorY-ring[b].coorY;
	cLine = sqrt(tx*tx+ty*ty);
	if(ring[a].radius+ring[b].radius > cLine && fabs(ring[a].radius-ring[b].radius) < cLine)
		return 1;
	if(ring[a].coorX==ring[b].coorX && ring[a].coorY==ring[b].coorY && ring[a].radius==ring[b].radius)
		return 1;
	return 0;
}

void link(int node, int *sum, int before)
{
	int i;

	if(before != -1)
		reach[node][before] = 0;
	*sum += 1;
	component[node] = *sum;
	for(i=0; i<num; i++){
		if(reach[node][i] && !component[i])
			link(i, sum, node);
	}
}

int main(void)
{
	int i, j;
	int sum;
	int max;

	while(scanf("%d", &num) != EOF){
		if(num == -1)
			break;
		initial();
		input();
		for(i=0; i<num; i++){
			for(j=i+1; j<num; j++)
				reach[i][j] = reach[j][i] = is_reach(i, j);
		}
		for(i=0; i<num; i++){
			if(!component[i]){
				sum = 0;
				link(i, &sum, -1);
			}
		}
		for(i=0,max=0; i<num; i++)
			if(component[i] > max)
				max = component[i];
		if(max == 1)
			printf("The largest component contains %d ring.\n", max);
		else
			printf("The largest component contains %d rings.\n", max);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
