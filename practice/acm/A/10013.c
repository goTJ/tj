/*   @JUDGE_ID:   10319NX   10013   C */
#include<stdio.h>

enum { D_MAX = 1000003 };


int main(void)
{
	char num[D_MAX];
	unsigned int length, i, j;
	int a, b;
	unsigned int time_max, time;

	scanf("%u", &time_max);
	for(time=0; time<time_max; time++){
		scanf("%u", &length);
		/*add*/
		for(i=1; i<=length+1; i++)
			num[i] = 0;
		for(i=length; i>0; i--){
			scanf("%d%d", &a, &b);
			num[i] = (char)a+(char)b;
		}
		/*gain*/
		for(i=1; i<=length; i++){
			while(num[i] > 9){
				num[i+1]++;
				num[i] -= 10;
			}
		}
		/*output*/
		if(time)
			printf("\n");
		if(num[length+1] != 0)
			printf("%d", (int)num[length+1]);
		for(i=length; i>0; i--){
			printf("%d", (int)num[i]);
		}
		printf("\n");
		/*end*/
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
