/* @JUDGE_ID:   10319NX 10189 C */
#include<stdio.h>
#include<string.h>

char map[105][105];
int way[8][2] = { {1,1}, {1,0}, {1,-1}, {0,1}, {0,-1}, {-1,1}, {-1,0}, {-1,-1} };

int main(void)
{
	int row, col;
	int i, j, k;
	int r, c;
	int times=0;

	while(scanf("%d%d", &row, &col) != EOF){
		if(!row && !col)
			break;
		if(times)
			printf("\n");
		memset(map, 0, sizeof(map));
		for(i=1; i<=row; i++)
			scanf("%s", &(map[i][1]));
		for(i=1; i<=row; i++){
			for(j=1; j<=col; j++){
				if(map[i][j] == '.'){
					map[i][j] = '0';
					for(k=0; k<8; k++){
						r = i+way[k][0];
						c = j+way[k][1];
						if(map[r][c] == '*')
							map[i][j]++;
					}
				}
			}
		}
		printf("Field #%d:\n", times+1);
		times++;
		for(i=1; i<=row; i++)
			printf("%s\n", &(map[i][1]));
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
