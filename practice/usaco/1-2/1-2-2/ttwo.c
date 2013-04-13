//### PROGRAM
/*
ID: flyherm003
PROG: ttwo
*/
#include<stdio.h>
#include<string.h>

char map[12][12];
int way[][2] = { -1,0, 0,1, 1,0, 0,-1 };
int used[1000][1000];
struct fly{
	int row;
	int col;
	int dir;
}obj[2]; /* 0: farmer, 1: cow */

void jjj(void)
{
	int i, j;

	for(i=0; i<12; i++){
		for(j=0; j<12; j++)
			printf("%c", map[i][j]);
		printf("\n");
	}
}

void find(int n, char sign)
{
	int i, j;

	for(i=1; i<=10; i++){
		for(j=1; j<=10; j++)
			if(map[i][j] == sign){
				obj[n].row = i;
				obj[n].col = j;
				obj[n].dir = 0;
		//		printf("(%d, %d) = %c\n", i, j, sign);
				return;
			}
	}
}

void initial(void)
{
	int i;

	find(0, 'F');
	find(1, 'C');
	memset(used, 0, sizeof(used));
	for(i=0; i<=11; i++){
		map[0][i] = '*';
		map[11][i] = '*';
		map[i][0] = '*';
		map[i][11] = '*';
	}
}

void go(int n)
{
	int tr, tc;
	
//	printf("\tgo go: %d\n", n);
//	printf("\t%d %d %d\n", obj[n].row, obj[n].col, obj[n].dir);
	tr = obj[n].row+way[obj[n].dir][0];
	tc = obj[n].col+way[obj[n].dir][1];
	if(map[tr][tc] == '*')
		obj[n].dir = (obj[n].dir+1)%4;
	else{
		obj[n].row = tr;
		obj[n].col = tc;
		map[tr][tc] = 'F';
	}
//	printf("\t%d %d %d\n", obj[n].row, obj[n].col, obj[n].dir);
}

int check(void)
{
	int a, b;

//	printf("go check\n");
//	printf("%d %d %d\n", obj[0].row-1, obj[0].col-1, 1<<obj[0].dir);
	a = (obj[0].row-1)*100+(obj[0].col-1)*10+(1<<obj[0].dir);
//	printf("%d %d %d\n", obj[1].row-1, obj[1].col-1, 1<<obj[1].dir);
	b = (obj[1].row-1)*100+(obj[1].col-1)*10+(1<<obj[1].dir);
//	printf("%d %d\n", a, b);
	if(used[a][b]){
		printf("0\n");
		return 0;
	}
	else
		used[a][b] = 1;
	return 1;
}

int main(void)
{
	int i, times;
	freopen("ttwo.in",  "r",  stdin);
	freopen("ttwo.out",  "w",  stdout);

	while(1){
		for(i=1; i<=10; i++)
			if(scanf("%s", map[i]+1) == EOF)
				exit(0);
		initial();
//		jjj();
		times = 0;
		while(check()){
		//	printf("gogogo %d\n", times);
			if(obj[0].row==obj[1].row && obj[0].col==obj[1].col){
				printf("%d\n", times);
				break;
			}
			for(i=0; i<2; i++)
				go(i);
		//	jjj();
			times++;
		}
	}
	return 0;
}
//### END
