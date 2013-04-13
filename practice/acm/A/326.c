/* @JUDGE_ID:   10319NX 326 C */
#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	int num[15];
	int lim, n, i, j;
	int t, tt;

	while(scanf("%d", &lim) != EOF){
		if(!lim)
			break;
		for(i=0; i<lim; i++){
			t = num[0];
			scanf("%d", &num[0]);
			for(j=1; j<=i; j++){
				tt = num[j];
				num[j] = num[j-1]-t;
				t = tt;
			}
		}
		scanf("%d", &n);
		for(i=0; i<n; i++){
			for(j=lim-2; j>=0; j--){
				num[j] = num[j]+num[j+1];
			}
		}
		printf("Term %d of the sequence is %d\n", lim+n, num[0]);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
