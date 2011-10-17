/* @JUDGE_ID:   10319NX 10032 C */
#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	int a[105], b[105];
	int la, lb, sa, sb;
	int n, i, j, t;

	while(scanf("%d", &n) != EOF){
		sa = sb = 0;
		for(la=0; la<n/2; la++){
			scanf("%d", &(a[la]));
			sa += a[la];
		}
		for(lb=0; lb<(n+1)/2; lb++){
			scanf("%d", &(b[lb]));
			sb += b[lb];
		}
		for(i=0; i<la; i++){
		   for(j=0; j<lb; j++){
			if(abs((sa-a[i]+b[j])-(sb-b[j]+a[i])) < abs(sa-sb)){
				sa = sa-a[i]+b[j];
				sb = sb-b[j]+a[i];
				t = a[i];
				a[i] = b[j];
				b[j] = t;
			}
		   }
		}
		if(sa < sb)
			printf("%d %d\n", sa, sb);
		else
			printf("%d %d\n", sb, sa);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
