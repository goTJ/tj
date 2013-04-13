/* @JUDGE_ID:   10319NX 567 C */
#include<stdio.h>

void initial(int way[][21])
{
	int i, j;

	for(i=1; i<=20; i++)
		for(j=1; j<=20; j++)
			way[i][j] = 5000;
}

void floyd(int way[][21])
{
	int i, j, k;
	int t;

	for(k=1; k<=20; k++)
		for(i=1; i<=20; i++)
			for(j=1; j<=20; j++)
				if((t=way[i][k]+way[k][j]) < way[i][j])
					way[i][j] = t;
}

int main(void)
{
	int way[21][21];
	int n, t, tt, set=1;
	int i, j;

	while(1){
		initial(way);
		for(i=1; i<20; i++){
			if(scanf("%d", &n) == EOF)
				return 0;
			for(j=0; j<n; j++){
				if(scanf("%d", &t) == EOF)
					return 0;
				way[i][t] = 1;
				way[t][i] = 1;
			}
		}
		floyd(way);
		printf("Test Set #%d\n", set);
		set++;
		scanf("%d", &n);
		for(i=0; i<n; i++){
			scanf("%d%d", &t, &tt);
			printf("%2d to %2d:%2d\n", t, tt, way[t][tt]);
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
