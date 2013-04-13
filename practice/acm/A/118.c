/* @JUDGE_ID:   10319NX 118 C */
#include<stdio.h>
#include<string.h>

int map[55][55];
int sign[256];
int way[4][2] = { 1,0, 0,1, -1,0, 0,-1 };
char dis[4] = { 'E', 'N', 'W', 'S' };
int X, Y;

void initial(void)
{
	memset(map, 0, sizeof(map));
	sign['E'] = 0;
	sign['N'] = 1;
	sign['W'] = 2;
	sign['S'] = 3;
}

int main(void)
{
	int x, y, d, tx, ty;
	int lost;
	char c;

	initial();
	scanf("%d %d\n", &X, &Y);
	while(scanf("%d %d %c\n", &x, &y, &c) != EOF){
		d = sign[c];
		lost = 0;
		while((c=getchar()) != EOF){
			if(c == '\n')
				break;
			else if(c == 'R')
				d = (d+4-1)%4;
			else if(c == 'L')
				d = (d+1)%4;
			else if(c == 'F'){
				tx = x+way[d][0];
				ty = y+way[d][1];
				if(tx>X || tx<0 || ty>Y || ty<0){
					if(!map[x][y]){
						map[x][y] = 1;
						lost = 1;
						break;
					}
				}
				else{
					x = tx;
					y = ty;
				}
			}
		}
		while(c != '\n')
			c = getchar();
		printf("%d %d %c", x, y, dis[d]);
		if(lost)
			printf(" LOST");
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
