//### PROGRAM
/*
ID: flyherm003
PROG: range
*/
#include<stdio.h>
#include<string.h>
#define N 255

int map[N][N];
int num[N];
int lim;

void transcript(void)
{
	int i, j;

	for(i=1; i<=lim; i++)
		for(j=1; j<=lim; j++)
			map[i][j] += map[i-1][j] + map[i][j-1] - map[i-1][j-1];
}

int main(void)
{
	int i, j, k;
	char buf[N];
/**/	freopen("range.in", "r", stdin);
/**/	freopen("range.out", "w", stdout);

	while(scanf("%d", &lim) != EOF){
		for(i=1; i<=lim; i++){
			scanf("%s", &buf[1]);
			for(j=1; j<=lim; j++)
				map[i][j] = (buf[j]=='1') ? 0 : 1;
		}
		memset(num, 0, sizeof(num));
		transcript();
		for(i=2; i<=lim; i++){
			for(j=2; j<=lim; j++)
				for(k=2; k<=i&&k<=j; k++)
					if(!(map[i][j]+map[i-k][j-k]-map[i][j-k]-map[i-k][j]))
						num[k]++;
		}
		for(i=2; i<=lim; i++)
			if(num[i])
				printf("%d %d\n", i, num[i]);
	}
	return 0;
}
//### END