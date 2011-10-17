/* @JUDGE_ID:   10319NX 10252 C */
#include<stdio.h>
#include<string.h>

int main(void)
{
	int tab[2][26];
	int i, j, min;
	char c;

	while(1){
		memset(tab, 0, sizeof(tab));
		for(i=0; i<2; i++){
			while((c=getchar()) != EOF){
				if(c == '\n')
					break;
				tab[i][c-'a']++;
			}
			if(c == EOF)
				return 0;
		}
		for(i=0; i<26; i++){
			min = (tab[0][i]<tab[1][i]) ? tab[0][i] : tab[1][i];
			for(j=0; j<min; j++)
				printf("%c", i+'a');
		}
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
