/* @JUDGE_ID:   10318MP 10223 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>
#include<string.h>
#include<limits.h>

unsigned int tab[35];

void initial(void)
{
	int i, j;

	memset(tab, 0, sizeof(tab));
	tab[0] = 1;
	tab[1] = 1;
	for(i=2; i<=21; i++){
		for(j=0; j<i; j++)
			tab[i] += tab[j]*tab[i-j-1];
	}
////	for(i=0; i<=21; i++)
////		printf("%u: %u\n", i, tab[i]);
}

int main(void)
{
	unsigned int num;
	int i;

	initial();
	while(scanf("%u", &num) != EOF){
		for(i=1; i<=21; i++)
			if(tab[i] == num){
				printf("%d\n", i);
				break;
			}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
