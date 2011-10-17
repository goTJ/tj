/* @JUDGE_ID:   10319NX 10415 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>
#include<ctype.h>

			// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10
int tab[2][7][11] = { {  { 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 } // a
			,{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 } // b
			,{ 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1 } // c
			,{ 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0 } // d
			,{ 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0 } // e
			,{ 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0 } // f
			,{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0 }}// g
		     ,{	 { 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 } // A
			,{ 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 } // B
			,{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } // C
			,{ 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0 } // D
			,{ 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0 } // E
			,{ 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0 } // F
			,{ 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 }}// G
		    };

int main(void)
{
	int status[11], times[11];
	int cases, round;

	scanf("%d\n", &cases);
	for(round=0; round<cases; round++){
		char c;
		int big, num;
		for(int i=1; i<=10; i++)
			status[i] = times[i] = 0;
		while((c=getchar()) != '\n'){
			big = isupper(c) ? 1 : 0;
			num = tolower(c)-'a';
//			printf("%c: %d %d\n", c, big, num);
			for(int i=1; i<=10; i++){
				if(status[i] < tab[big][num][i])
					times[i]++;
				status[i] = tab[big][num][i];
			}
		}
		printf("%d", times[1]);
		for(int i=2; i<=10; i++)
			printf(" %d", times[i]);
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
