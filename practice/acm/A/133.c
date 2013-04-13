/* @JUDGE_ID:   10319NX 133 C */
#include<stdio.h>

int num[22];
int lim;

int c(int n)
{
	return (n+lim)%lim;
}

void initial(void)
{
	int i;

	for(i=0; i<lim; i++)
		num[i] = 1;
}

int main(void)
{
	int a, b;
	int pa, pb;
	int i, flag;
	int ta, tb;

	while(scanf("%d%d%d", &lim, &a, &b) != EOF){
		if(!lim && !a && !b)
			break;
		flag = 0;
		initial();
		pa = lim-1;
		pb = 0;
		while(flag < lim){
			ta = a%(lim-flag);
			tb = b%(lim-flag);
			if(!ta)
				ta = lim-flag;
			if(!tb)
				tb = lim-flag;
			if(flag)
				printf(",");
			do{
				pa = c(pa+1);
			}while(!num[pa]);
			for(i=1,pa=c(pa+1); i<ta; pa=c(pa+1)){
				if(num[pa])
					i++;
			}
			pa = c(pa-1);
			printf("%3d", pa+1);
			flag++;
			do{
				pb = c(pb-1);
			}while(!num[pb]);
			for(i=1, pb=c(pb-1); i<tb; pb=c(pb-1)){
				if(num[pb])
					i++;
			}
			pb = c(pb+1);
			if(pb != pa){
				printf("%3d", pb+1);
				flag++;
			}
			num[pa] = 0;
			num[pb] = 0;
		}
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
