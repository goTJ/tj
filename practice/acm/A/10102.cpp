/* @JUDGE_ID:   10319NX 10102 C++ ¦ó¦Ì¯S^^ */
#include<stdio.h>
#include<stdlib.h>

typedef struct coor{
	int x;
	int y;
};
coor three[5000];
coor one[5000];
int ooo, ttt;

int main(void)
{
	int n, i, j;
	char in[100];
	int max, min;
	int t;

	while(scanf("%d", &n) != EOF){
		ooo = ttt = 0;
		for(i=0; i<n; i++){
			scanf("%s", in);
			for(j=0; j<n; j++){
				if(in[j] == '1'){
					one[ooo].x = i;
					one[ooo++].y = j;
				}
				else if(in[j] == '3'){
					three[ttt].x = i;
					three[ttt++].y = j;
				}
			}
		}
		max = 0;
		for(i=0; i<ooo; i++){
			min = abs(three[0].x-one[i].x)+abs(three[0].y-one[i].y);
			for(j=1; j<ttt; j++){
				if((t=abs(three[j].x-one[i].x)+abs(three[j].y-one[i].y)) < min)
					min = t;
			}
			if(min > max)
				max = min;
		}
		printf("%d\n", max);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
