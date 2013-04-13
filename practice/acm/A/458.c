/* @JUDGE_ID:   10319NX 458 C */
#include<stdio.h>

int main(void)
{
	char c;

	while((c=getchar()) != EOF){
		if(c == '\n')
			printf("\n");
		else
			printf("%c", c-7);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
