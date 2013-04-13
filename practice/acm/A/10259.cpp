/* @JUDGE_ID:   10319NX 10259 C++ "§Ú°Q¹½ÂÅ¤Ä>.<" */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int map[105][105];
int range;
struct fly{
	int val;
	int row;
	int col;
}val[10050];
int lim;
int max[105][105];
int used[105][105];
int way[4][2] = { {1,0}, {0,1}, {-1,0}, {0,-1} };

int comp(const void *p, const void *q)
{
	return ((struct fly *)p)->val - ((struct fly *)q)->val;
}

int main(void)
{
	int i, j, k, p;
	int t, tr, tc;
	int maxx;
	int times, round;

	scanf("%d", &round);
	for(times=0; times<round; times++){
		scanf("%d%d", &lim, &range);
		memset(used, 0, sizeof(used));
		for(i=0,p=0; i<lim; i++){
			for(j=0; j<lim; j++){
				scanf("%d", &map[i][j]);
				max[i][j] = map[i][j];
				val[p].val = map[i][j];
				val[p].row = i;
				val[p++].col = j;
			}
		}
		qsort(val, p, sizeof(val[0]), comp);
		for(i=0; i<p; i++)
			if(val[i].row==0 && val[i].col==0)
				break;
		used[val[i].row][val[i].col] = 1;
		for(; i<p; i++){
			if(used[val[i].row][val[i].col]){
				for(j=0; j<4; j++){
					tr = val[i].row;
					tc = val[i].col;
					for(k=0; k<range; k++){
						tr += way[j][0];
						tc += way[j][1];
						if(tr<0 || tr>=lim || tc<0 || tc>=lim)
							break;
						if(map[tr][tc] > map[val[i].row][val[i].col]){
							if((t=max[val[i].row][val[i].col]+map[tr][tc]) > max[tr][tc]){
								max[tr][tc] = t;
								used[tr][tc] = 1;
							}
						}
					}
				}
			}
		}
		if(times)
			printf("\n");

		maxx = 0; 
		for(i=0; i<lim; i++)
			for(j=0; j<lim; j++)
				if(used[i][j] && max[i][j]>maxx)
					maxx = max[i][j];
		printf("%d\n", maxx);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
