#include<stdio.h>

int RPS(char a, char b)
{
	if(a == b)
		return 0;
	else if((a=='R' && b=='S') || (a=='S' && b=='P')  || (a=='P' && b=='R'))
		return 1;
	else
		return -1;
}

int main()
{
	int way[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
	char map[106][106];
	char tmp[106][106];
	int round, times;
	int row, col;
	int days, d;
	int r, c;
	int tr, tc;
	int i;

	scanf("%d", &times);
	for(round=0; round<times; round++){
		scanf("%d%d%d", &row, &col, &days);
		for(r=0; r<row; r++)
			scanf("%s", map[r]);
		for(d=0; d<days; d++){
			for(r=0; r<row; r++){
				for(c=0; c<col; c++){
					tmp[r][c] = map[r][c];
					for(i=0; i<4; i++){
						tr = r+way[i][0];
						tc = c+way[i][1];
						if(tr < row && tr >=0 && tc < col && tc >= 0){
							if(RPS(map[tr][tc], map[r][c]) > 0)
								tmp[r][c] = map[tr][tc];
						}
					}
				}
			}
			for(r=0; r<row; r++){
				for(c=0; c<col; c++)
					map[r][c] = tmp[r][c];
			}
		}
		if(round)
			printf("\n");
		for(r=0; r<row; r++)
			printf("%s\n", map[r]);
	}
	return 0;
}
