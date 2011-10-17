/* @JUDGE_ID:   10319NX 141 C */
#include<stdio.h>
#include<string.h>

int map[55][55];
int record[405][55][55];
int lim, num;
int sign[256];

void initial(void)
{
	memset(map, 0, sizeof(map));
	lim = 0;
	sign['+'] = 1;
	sign['-'] = 0;
}

int check(void)
{
	int i, j, k;
	int flag;

	for(i=0; i<lim; i++){
		flag = 1;
		for(j=0; j<num; j++){
			for(k=0; k<num; k++){
				if(map[j][k] != record[i][j][k]){
					flag = 0;
					break;
				}
			}
			if(!flag)
				break;
		}
		if(flag)
			return 1;
	}
	return 0;
}

void insert(void)
{
	int i, j;

	for(i=0; i<num; i++){
		for(j=0; j<num; j++){
			record[lim][i][j] = map[i][j];
			record[lim+1][i][j] = map[num-j-1][i];
			record[lim+2][i][j] = map[num-i-1][num-j-1];
			record[lim+3][i][j] = map[j][num-i-1];
		}
	}
	lim += 4;
}

int main(void)
{
	int i;
	int x, y;
	char act;
	int win;

	while(scanf("%d\n", &num) != EOF){
		if(!num)
			break;
		initial();
		win = 0;
		for(i=0; i<num*2; i++){
			scanf("%d %d %c\n", &x, &y, &act);
			x--;
			y--;
			map[x][y] = sign[act];
			if(check()){
				printf("Player %d wins on move %d\n", ((i-1)%2)+1, i+1);
				win = 1;
				break;
			}
			insert();
		}
		if(win){
			for(i++; i<num*2; i++)
				scanf("%d %d %c\n", &x, &y, &act);
		}
		else
			printf("Draw\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
