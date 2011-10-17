/* @JUDGE_ID:   10319NX 108 C */
#include<stdio.h>

int map[105][105];
int lim;

int main(void)
{
	int i, j, ii, jj;
	int max, t;

	while(scanf("%d", &lim) != EOF){
		for(i=1; i<=lim; i++){
			for(j=1; j<=lim; j++)
				scanf("%d", &map[i][j]);
		}
		max = map[1][1];
		for(i=1; i<=lim; i++){
			for(j=1; j<=lim; j++){
				map[i][j] += map[i-1][j]+map[i][j-1]-map[i-1][j-1];
				if(map[i][j] > max)
					max = map[i][j];
			}
		}
		for(ii=1; ii<=lim; ii++){
			for(jj=1; jj<=lim; jj++){
				for(i=0; i<ii; i++){
					for(j=0; j<jj; j++){
						t = map[i][j]+map[ii][jj]-map[i][jj]-map[ii][j];
						if(t > max)
							max = t;
					}
				}
			}
		}
		printf("%d\n", max);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
