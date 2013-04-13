/* @JUDGE_ID:   10319NX 483 C */
#include<stdio.h>
#include<string.h>

int main(void)
{
	char s[1000];
	char c;
	int len, i;

	while(scanf("%s", s) != EOF){
		len = strlen(s);
		for(i=len-1; i>=0; i--)
			putchar(s[i]);
		if((c=getchar()) != EOF)
			putchar(c);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
