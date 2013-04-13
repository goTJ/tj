/* @JUDGE_ID:   10319NX 575 C */
#include<stdio.h>
#include<ctype.h>

int main(void)
{
	double sum=0.0;
	char c;
	int flag=0, num=0;

	while((c=getchar()) != EOF){
		if(c == '\n'){
			printf("%.0lf\n", sum);
			sum = 0.0;
			num = 0;
			flag = 1;
		}
		else if(c=='0' && flag)
			return 0;
		else if(!isspace(c)){
			num += c-'0';
			sum = sum*2.0+(double)num;
			flag = 0;
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
