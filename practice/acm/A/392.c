/* @JUDGE_ID:   10319NX 392 C */
#include<stdio.h>

void print_x(int ex)
{
	if(ex)
		printf("x");
	if(ex > 1)
		printf("^%d", ex);
}

int main(void)
{
	int ex, num;
	int i, j;

	while(1){
		ex = 8;
		if(scanf("%d", &num) == EOF)
			return 0;
		while(!num && ex){
			ex--;
			scanf("%d", &num);
		}
		if(num < 0){
			printf("-");
			num *= -1;
		}
		if(num>1 || !ex)
			printf("%d", num);
		print_x(ex);
		ex--;
		for(; ex>=0; ex--){
			scanf("%d", &num);
			if(num){
				if(num < 0){
					printf(" - ");
					num *= -1;
				}
				else
					printf(" + ");
				if(num>1 || !ex)
					printf("%d", num);
				print_x(ex);
			}
		}
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
