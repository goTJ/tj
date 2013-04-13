/*###########################################################################*/
/*       title: Unidirectional TSP                                           */
/*                                                                           */
/*           start: ?                                                        */
/*                                                                           */
/*                finish: 2001年  6月27日 周三 09時48分09秒 CST              */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   116   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>

int map[101][11][2];
int row, col;

int circle(int a)
{
	return (a+col)%col;
}

void gogogo(int r, int c)
{
	int i, n = circle(c-1), min, t;

	min = map[r+1][n][0];
	map[r][c][1] = n;
	for(i=0; i<2; i++){
		n = circle(c+i);
		if((t=map[r+1][n][0])<min || (t==min&& n<map[r][c][1])){
			min = t;
			map[r][c][1] = n;
		}
	}
	map[r][c][0] += min;	
}

void find_min(void)
{
	int i, min, t, pos=0;

	min = map[0][0][0];
	for(i=1; i<col; i++)
		if((t=map[0][i][0]) < min){
			min = t;
			pos = i;
		}
	printf("%d", pos+1);
	for(i=0; i<row-1; i++){
		printf(" %d", (t=map[i][pos][1])+1);
		pos = t;
	}
	printf("\n%d\n", min);
}

int main(void)
{
	int i, j;

	while(scanf("%d%d", &col, &row) == 2){
		for(i=0; i<col; i++)
			for(j=0; j<row; j++)
				scanf("%d", &map[j][i][0]);
		for(i=row-2; i>=0; i--)
			for(j=0; j<col; j++)
				gogogo(i, j);
		find_min();
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
