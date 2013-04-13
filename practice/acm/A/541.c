/*   @JUDGE_ID:   10387ZJ   541   C */
#include<stdio.h>
#include<string.h>

int map[100][100];
int size;
int row[100], column[100];
char ans[2][100];

int d_row(int n)
{
	int i, sum = 0;

	for(i=0; i<size; i++)
		sum += map[n][i];
	return sum;
}

int d_column(int n)
{
	int i, sum = 0;

	for(i=0; i<size; i++)
		sum += map[i][n];
	return sum;
}

int only(char *n, int *way)
{
	int i, sum = 0;

	for(i=0; i<size; i++)
		if(n[i]){
			sum++;
			*way = i;
		}
	return sum;
}

void judge(void)
{
	int i, j, flag = 1, x, y;

	for(i=0; i<size; i++){
		ans[0][i] = row[i]%2;
		ans[1][i] = column[i]%2;
		if(row[i]%2)
			flag = 0;
		if(column[i]%2)
			flag = 0;
	}
	/*
	printf("flag: %d\n", flag);
	for(i=0; i<size; i++){
		for(j=0; j<size; j++)
			printf("%d ", map[i][j]);
		printf("\n");
	}
	for(i=0; i<size; i++)
		printf("row: %d   column: %d\n", row[i], column[i]);
	for(i=0; i<size; i++)
		printf("ans: %d %d\n", ans[0][i], ans[1][i]);
	*/
	if(flag){
		printf("OK\n");
		return;
	}
	else if(only(&(ans[0][0]), &x) == 1 && only(&(ans[1][0]), &y) == 1){
		printf("Change bit (%d,%d)\n", x+1, y+1);
		return;
	}
	else
		printf("Corrupt\n");
}

int main(void)
{
	int i, j;

	while(scanf("%d", &size) != EOF){
		if(!size)
			exit(0);
		for(i=0; i<size; i++)
			for(j=0; j<size; j++)
				scanf("%d", &(map[i][j]));
		for(i=0; i<size; i++)
			row[i] = d_row(i);
		for(i=0; i<size; i++)
			column[i] = d_column(i);
		judge();
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
