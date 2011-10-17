//### PROGRAM
/*
ID: flyherm003
PROG: preface
*/
#include<stdio.h>
#include<string.h>

enum { I, V, X, L, C, D, M, MAX };
int tab[MAX];
int sign[] = { 'I', 'V', 'X', 'L', 'C', 'D', 'M' };
int site[3][3]={ I, V, X,
		 X, L, C,
		 C, D, M };
int val[11][3]={ 0, 0, 0,
		 1, 0, 0, /*  1 */
		 2, 0, 0, /*  2 */
		 3, 0, 0, /*  3 */
		 1, 1, 0, /*  4 */
		 0, 1, 0, /*  5 */
		 1, 1, 0, /*  6 */
		 2, 1, 0, /*  7 */
		 3, 1, 0, /*  8 */
		 1, 0, 1, /*  9 */
		 0, 0, 1 /* 10 */ };

void count(int n)
{
	int i, tmp, j;

	for(i=0; n>0&&i<3; i++,n/=10){
		tmp = n%10;
		for(j=0; j<3; j++)
			tab[site[i][j]] += val[tmp][j];
	}
	tab[M] += n;
}

int main(void)
{
	int num, i;
	freopen("preface.in",  "r",  stdin);
	freopen("preface.out",  "w",  stdout);

	while(scanf("%d", &num) != EOF){
		memset(tab, 0, sizeof(tab));
		for(i=1; i<=num; i++)
			count(i);
		for(i=0; i<MAX; i++)
			if(tab[i])
				printf("%c %d\n", sign[i], tab[i]);
	}
	return 0;
}
//### END
