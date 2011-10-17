/* @JUDGE_ID:   10319NX 10193 C */
#include<stdio.h>

int main(void)
{
	int round, times;
	int a, b, tmp, i;
	char s[50];

	scanf("%d", &round);
	for(times=1; times<=round; times++){
		a = b = 0;
		scanf("%s", s);
		for(i=0; s[i]; i++)
			a = a*2+s[i]-'0';
		scanf("%s", s);
		for(i=0; s[i]; i++)
			b = b*2+s[i]-'0';
		while(b){
			tmp = a%b;
			a = b;
			b = tmp;
		}
		if(a == 1)
			printf("Pair #%d: Love is not all you need!\n", times);
		else
			printf("Pair #%d: All you need is love!\n", times);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
