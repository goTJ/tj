/* @JUDGE_ID:   10319NX 10116 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>
#include<string.h>

int map[12][12];
int used[12][12];
int way[4][2] = { {1,0}, {0,1}, {-1,0}, {0,-1} };
int sign[256];
int row, col;

void initial(void)
{
	sign['S'] = 0;
	sign['E'] = 1;
	sign['N'] = 2;
	sign['W'] = 3;
}

int main(void)
{
	int i, j;
	char in[13];
	int r, c, tr, tc;
	int step=0;

	initial();
	while(scanf("%d%d%d", &row, &col, &c) != EOF){
		if(!row && !col && !c)
			break;
		r = 0;
		c--;
		memset(used, 0, sizeof(used));
		for(i=0; i<row; i++){
			scanf("%s", in);
			for(j=0; j<col; j++)
				map[i][j] = sign[in[j]];
		}
		step = 1;
		while(r>=0 && r<row && c>=0 && c<col && !used[r][c]){
			used[r][c] = step;
			step++;
			tr = r+way[map[r][c]][0];
			tc = c+way[map[r][c]][1];
			r = tr;
			c = tc;
		}
		if(r<0 || r>=row || c<0 || c>=col)
			printf("%d step(s) to exit\n", step-1);
		else
			printf("%d step(s) before a loop of %d step(s)\n", used[r][c]-1, step-used[r][c]);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
