//### PROGRAM
/*
ID: flyherm003
PROG: castle
start: 
finish: 
*/
#include<stdio.h>
#include<string.h>

int map[52][52];
int go[52][52][4];
int row_max, col_max;
int space;
int area[2502];
int way[][2] = { 0,-1, -1,0, 0,1, 1,0 };

void initial(void)
{
	memset(map, 0, sizeof(map));
	memset(go, 0, sizeof(go));
	memset(area, 0, sizeof(area));
	space = 1;
}

void input(void)
{
	int i, j, k, tmp;

		for(i=0; i<row_max; i++)
			for(j=0; j<col_max; j++){
				scanf("%d", &tmp); 
				for(k=0; k<4; k++){
					go[i][j][k] = tmp&1;
					tmp >>= 1;
				}
			}
}

int find_room(int row, int col)
{
	int i, sum=1;

	if(map[row][col])
		return 0;
	else{
		map[row][col] = space;
		for(i=0; i<4; i++)
			if(!go[row][col][i])
				sum += find_room(row+way[i][0], col+way[i][1]);
		return sum;
	}
}

int get(int row, int col, char *c, int *max)
{
	int i, sign=map[row][col], par;

	if(row-1>=0 && (par=map[row-1][col])!=sign)
		if(area[par] > *max){
			*max = area[par]+area[sign];
			*c = 'N';
			return 1;
		}
	if(col+1<col_max && (par=map[row][col+1])!=sign)
		if(area[par] > *max){
			*max = area[par]+area[sign];
			*c = 'E';
			return 1;
		}
	return 0;
}

int main(void)
{
	int i, j, tmp;
	int max, dr, dc;
	char sign='E', lala;
	//freopen("castle.in",  "r",  stdin);
	//freopen("castle.out",  "w",  stdout);

	while(scanf("%d%d", &col_max, &row_max) != EOF){
		initial();
		input();
		for(i=0; i<row_max; i++)
			for(j=0; j<col_max; j++)
				if( (tmp=find_room(i, j)) )
					area[space++] = tmp;
		max = area[1];
		for(i=2; i<space; i++)
			if(area[i] > max)
				max = area[i];
		printf("%d\n", space-1);
		printf("%d\n", max);
		max = 0;
		for(j=0; j<col_max; j++)
			for(i=row_max-1; i>=0; i--){
				if(get(i, j, &sign, &max)){
					dr = i;
					dc = j;
				}
			}
		printf("%d\n", max);
		printf("%d %d %c\n", dr+1, dc+1, sign);
	}
	return 0;
}
//### END
