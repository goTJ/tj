//### PROGRAM
/*
ID: flyherm003
PROG: inflate
*/
#include<stdio.h>
#include<string.h>

struct fly{
	int point;
	int min;
}cate[10005];
int min[10005];
int l_min, l_cate;

void initial(void)
{
	memset(min, 0, sizeof(min));
}

int main(void)
{
	int i, j, max;
////	freopen("inflate.in", "r", stdin);
////	freopen("inflate.out", "w", stdout);

	while(scanf("%d%d", &l_min, &l_cate) != EOF){
		for(i=0; i<l_cate; i++)
			scanf("%d%d", &(cate[i].point), &(cate[i].min));
		initial();
		for(i=0; i<l_cate; i++){
		   for(j=l_min-cate[i].min; j>=0; j--){
			if(min[j]+cate[i].point > min[j+cate[i].min])
				min[j+cate[i].min] = min[j]+cate[i].point;
		   }
		}
		max = 0;
		for(i=0; i<=l_min; i++)
			if(min[i] > max)
				max = min[i];
		printf("%d\n", max);
	}
}
//### END
