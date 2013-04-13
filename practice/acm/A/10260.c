/* @JUDGE_ID:   10319NX 10260 C */
/*
 *       1 represents B, F, P, or V
 *       2 represents C, G, J, K, Q, S, X,  or Z
 *       3 represents D or T
 *       4 represents L
 *       5 represents M or N
 *       6 represents R
 */
#include<stdio.h>

/*		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z */
int tab[26] = { 0, 1, 2, 3, 0, 1, 2, 0, 0, 2, 2, 4, 5, 5, 0, 1, 2, 6, 2, 3, 0, 1, 0, 2, 0, 2 };

int main(void)
{
	char in[50];
	int i;
	int last;

	while(scanf("%s", in) != EOF){
		last = 0;
		for(i=0; in[i]; i++){
			if(tab[in[i]-'A']!=last && tab[in[i]-'A'])
				printf("%d", tab[in[i]-'A']);
			last = tab[in[i]-'A'];
		}
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
