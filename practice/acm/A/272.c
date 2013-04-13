/*   @JUDGE_ID:   10387ZJ   272   C  */
#include<stdio.h>

int main(void)
{
	char c;
	int n = 1;

	while(scanf("%c", &c) != EOF){
		if(c=='"' && n){
			printf("``");
			n = 0;
		}
		else if(c=='"' && !n){
			printf("''");
			n = 1;
		}
		else
			printf("%c", c);
	}
}
/* @END_OF_SOURCE_CODE */ 
