/* @JUDGE_ID:	10319NX	572  C++*/
#include<stdio.h>

char map[105][105];
int row, col;
int way[8][2] = { {1,0}, {1,-1}, {1,1}, {0,1}, {0,-1}, {-1,1}, {-1,0}, {-1,-1} };

void searchFellow(int r, int c)
{
	int i;
	int tr, tc;

	map[r][c] = '*';
	for(i=0; i<8; i++){
		tr = r + way[i][0];
		tc = c + way[i][1];
		if(map[tr][tc] == '@')
			searchFellow(tr, tc);
	}
}

int main()
{
	int i, j;
	int num;

	while(scanf("%d%d", &row, &col) != EOF){
		if(!row || !col)
			break;
		num = 0;
		for(i=1; i<=row; i++)
			scanf("%s", &map[i][1]);
		for(i=1; i<=row; i++){
			for(j=1; j<=col; j++){
				if(map[i][j] == '@'){
					num++;
					searchFellow(i, j);
				}
			}
		}
		printf("%d\n", num);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
