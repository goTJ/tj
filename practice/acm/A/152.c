/* @JUDGE_ID:   10319NX 152 C */
#include<stdio.h>
#include<string.h>

struct fly{
	int x;
	int y;
	int z;
}tree[5005];
int lim=0;
int unit[11];

void initial(void)
{
	int i;

	for(i=0; i<11; i++)
		unit[i] = i*i;
}

int apart(int a, int b)
{
	if(a > b)
		return a-b;
	else
		return b-a;
}

int d(int a, int b)
{
	int tx, ty, tz;

	tx = apart(tree[a].x, tree[b].x);
	ty = apart(tree[a].y, tree[b].y);
	tz = apart(tree[a].z, tree[b].z);
	return tx*tx+ty*ty+tz*tz;
}

int main(void)
{
	int i, j;
	int tx, ty, tz;
	int min, t;
	int histogram[10];

	initial();
	memset(histogram, 0, sizeof(histogram));
	while(scanf("%d%d%d", &tx, &ty, &tz) != EOF){
		if(!tx && !ty && !tz)
			break;
		tree[lim].x = tx;
		tree[lim].y = ty;
		tree[lim++].z = tz;
	}
	for(i=0; i<lim; i++){
		min = 200000;
		for(j=0; j<lim; j++)
			if(i != j){
				t = d(i, j);
				if(t < min){
					min = t;
				}
			}
		for(j=0; min>=unit[j]&&j<11; j++);
		if(j != 11)
			histogram[j-1]++;
	}
	for(i=0; i<10; i++)
		printf("%4d", histogram[i]);
	printf("\n");
	return 0;
}
/* @END_OF_SOURCE_CODE */
