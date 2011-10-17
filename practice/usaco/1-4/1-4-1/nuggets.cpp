//### PROGRAM
/*
ID: flyherm003
PROG: nuggets
*/
#include<stdio.h>

int main(void)
{
////	freopen("nuggets.in", "r", stdin);
////	freopen("nuggets.out", "w", stdout);
	bool change[257]={0};
	int i, j, k;
	int n[10], lim;
	int max=0, num=0, tmp;
	int last;
	int t;

	scanf("%d", &lim);
	for(i=0; i<lim; i++){
		scanf("%d", &n[i]);
		if(n[i] > max)
			max = n[i];
	}
	change[0] = true;
	num++;
	for(i=0; num<max; i++){
		tmp = num;
		for(j=0; j<max; j++){
			if(!change[j]){
				last = j;
				for(k=0; k<lim; k++){
					t = (j-n[k] >= 0) ? (j-n[k]) : (j-n[k]+max);
					if(change[t]){
						change[j] = true;
						num++;
						break;
					}
				}
			}
		}
		/*
		for(j=0; j<max; j++)
			printf("%d ", change[j]);
		printf("\n");
		*/
		if(tmp == num)
			break;
	}
	if(tmp == num)
		printf("0\n");
	else
		printf("%d\n", max*((i-2>=0)?i-2:0)+last);
	return 0;
}
//### END
