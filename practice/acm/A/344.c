/*###########################################################################*/
/*       title: Roman                                                        */
/*                                                                           */
/*           start: 2001年  7月12日 周四 19時00分27秒 CST                    */
/*                                                                           */
/*                finish: 2001年  7月12日 周四 19時31分46秒 CST              */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   344   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>

enum { I, V,  X,  L,   C, MAX };
/*     1  5  10  50  100               */
int tab[10];
int pos[2][3] = 
{ 
  I, V, X,
  X, L, C 
};

int val[10][3] = 
/* I, V, X, */
{
   0, 0, 0,
   1, 0, 0,
   2, 0, 0,
   3, 0, 0,
   1, 1, 0,
   0, 1, 0,
   1, 1, 0,
   2, 1, 0,
   3, 1, 0,
   1, 0, 1,
};

void count(int n)
{
	int i, j, k;

	for(i=0; i<2 && n>0; i++, n/=10){
		k = n%10;
		for(j=0; j<3; j++)
			tab[pos[i][j]] += val[k][j];
	}
/*	tab[C] += n;  */
}

int main(void)
{
	int i, n;
	char mode[MAX] = { 'i', 'v', 'x', 'l', 'c' };

	while(scanf("%d", &n) == 1){
		if(n==0)
			exit(0);
		memset(tab, 0, sizeof(tab));
		printf("%d:", n);
		for(i=1; i<=n; i++)
			count(i);
		for(i=0; i<MAX-1; i++)
			printf(" %d %c,", tab[i], mode[i]);
		printf(" %d %c\n", tab[i], mode[i]);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
