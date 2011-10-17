/*   @JUDGE_ID:   10319NX   494   C */
#include<stdio.h>

int main(void)
{
	char c, count = 0, flag;

	while(scanf("%c", &c) != EOF){
		flag = 0;
		if(c == '\n'){
			printf("%d\n", count);
			count = 0;
		}
		while(c>='a'&&c<='z' || c>='A'&&c<='Z'){
			flag = 1;
			scanf("%c", &c);
		}
		if(flag){
			count++;
			flag = 0;
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
