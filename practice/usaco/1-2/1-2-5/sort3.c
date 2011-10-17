//### PROGRAM
/*
ID: flyherm003
PROG: sort3
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int tab[3][3];

int main(void)
{
	int in[1005];
	int len, i, k;
	int num[3];
////	freopen("sort3.in", "r", stdin);
////	freopen("sort3.out", "w", stdout);

	while(scanf("%d", &len) != EOF){
		memset(num, 0, sizeof(num));
		memset(tab, 0, sizeof(tab));
		for(i=0; i<len; i++){
			scanf("%d", &in[i]);
			in[i]--;
			num[in[i]]++;
		}
		for(i=1; i<3; i++)
			num[i] += num[i-1];
		for(k=0,i=0; k<3; k++){
			for(; i<num[k]; i++){
				tab[k][in[i]]++;
			}
		}
		if(tab[1][0] > tab[0][1])
			printf("%d\n", tab[1][0]+tab[2][0]+tab[1][0]-tab[0][1]+tab[1][2]);
		else
			printf("%d\n", tab[1][0]+tab[2][0]+tab[0][1]-tab[1][0]+tab[2][1]);
	}
	return 0;
}
//### END
