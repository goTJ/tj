/* @JUDGE_ID:   10319NX 499 C */
#include<stdio.h>
#include<string.h>
#include<ctype.h>

int find_large(int n[][27])
{
	int i, j;
	int max=0;

	for(i=0; i<2; i++)
		for(j=0; j<26; j++)
			if(n[i][j] > max)
				max = n[i][j];
	return max;
}

int main(void)
{
	int alpha[2][27];
	int max, i, j;
	char c;
	char sign[2] = { 'A', 'a' };

	memset(alpha, 0, sizeof(alpha));
	while((c=getchar()) != EOF){
		if(c == '\n'){
			max = find_large(alpha);
			for(i=0; i<2; i++)
				for(j=0; j<26; j++)
					if(alpha[i][j] == max)
						printf("%c", j+sign[i]);
			printf(" %d\n", max);
			memset(alpha, 0, sizeof(alpha));
		}
		else if(isupper(c))
			alpha[0][c-'A']++;
		else if(islower(c))
			alpha[1][c-'a']++;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
