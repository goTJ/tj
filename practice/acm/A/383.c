/*###########################################################################*/
/*       title: Shipping                                                     */
/*                                                                           */
/*           start: 2001年  7月 2日 周一 17時18分55秒 CST                    */
/*                                                                           */
/*                finish: 2001年  7月 2日 周一 18時05分20秒 CST              */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   383   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>
#include<limits.h>

int path[32][32];
int num;
struct fly{
	char code[3];
}mode[32];

int sum(int a, int b)
{
	if(a==INT_MAX || b==INT_MAX)
		return INT_MAX;
	return a+b;
}

int tell(char tmp[])
{
	int i, j, flag;

	for(i=0; i<num; i++){
		flag = 0;
		for(j=0; j<2; j++)
			if(tmp[j] == mode[i].code[j])
				flag++;
		if(flag == 2)
			return i;
	}
}

void floyd(void)
{
	int i, j, k, t;

	for(k=0; k<num; k++)
		for(i=0; i<num; i++)
			for(j=0; j<num; j++)
				if((t=sum(path[i][k], path[k][j])) < path[i][j])
					path[i][j] = t;
}

int main(void)
{
	int t, times, P, N, i, j, a, b, ton;
	char tmp[3], tmpp[3];

	scanf("%d", &times);
	printf("SHIPPING ROUTES OUTPUT\n\n");
	for(t=0; t<times; t++){
		printf("DATA SET  %d\n\n", t+1);
		scanf("%d%d%d", &num, &P, &N);
		for(i=0; i<num; i++){
			scanf("%s", mode[i].code);
			for(j=0; j<num; j++)
				path[i][j] = INT_MAX;
		}
		for(i=0; i<P; i++){
			scanf("%s %s", tmp, tmpp);
			a = tell(tmp);
			b = tell(tmpp);
			path[a][b] = 1;
			path[b][a] = 1;
		}
		floyd();
		for(i=0; i<N; i++){
			scanf("%d %s %s", &ton, tmp, tmpp);
			a = tell(tmp);
			b = tell(tmpp);
			if(path[a][b] == INT_MAX)
				printf("NO SHIPMENT POSSIBLE\n");
			else
				printf("$%d00\n", ton*path[a][b]);
		}
		printf("\n");
	}
	printf("END OF OUTPUT\n");
	return 0;
}
/* @END_OF_SOURCE_CODE */
