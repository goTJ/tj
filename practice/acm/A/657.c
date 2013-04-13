/*###########################################################################*/
/*       title: The                                                          */
/*                                                                           */
/*           start: */
/*                                                                           */
/*                finish: */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   657   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>
#include<string.h>

int way[][2] = { 1,0, 0,-1, 0,1, -1,0 };
int max_row, max_col;
char map[52][52];
int ans[700], ans_lim;
int fly;

int area(char c)
{
	if(c=='*' || c=='X')
		return 1;
	return 0;
}

void bug(int row, int col)
{
	int tr, tc, i;

	map[row][col] = '*';
	for(i=0; i<4; i++){
		tr = row+way[i][0];
		tc = col+way[i][1];
		if(map[tr][tc] == 'X'){
			bug(tr, tc);
		}
	}
}

void count(int row, int col)
{
	int i, tr, tc;

	if(map[row][col] == 'X'){
		bug(row, col);
		fly++;
	}
	map[row][col] = '.';
	for(i=0; i<4; i++){
		tr = row+way[i][0];
		tc = col+way[i][1];
		if(area(map[tr][tc]))
			count(tr, tc);
	}
}

int comp(const void *p, const void *q)
{
	return *(int *)p - *(int *)q;
}

int main(void)
{
	int i, j, times=1;

	while(scanf("%d%d", &max_col, &max_row) == 2){
		if(max_col==0 || max_row==0)
			exit(0);
		memset(ans, 0, sizeof(ans));
		fly = ans_lim = 0;
		for(i=0; i<max_row; i++)
			scanf("%s", map[i]);
		for(i=0; i<max_row; i++){
			for(j=0; j<max_col; j++)
				if(area(map[i][j])){
					count(i, j);
					ans[ans_lim++] = fly;
					fly = 0;
				}
		}
		qsort(ans, ans_lim, sizeof(ans[0]), comp);
		printf("Throw %d\n", times++);
		for(i=0; i<ans_lim-1; i++)
			printf("%d ", ans[i]);
		printf("%d\n\n", ans[i]);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
