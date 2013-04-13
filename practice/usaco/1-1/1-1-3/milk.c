//###PROGRAM
/*
ID: flyherm003
PROG: milk
*/
#include<stdio.h>
#include<limits.h>

int main(void)
{
	FILE *in = fopen("milk.in", "r");
	FILE *out = fopen("milk.out", "w");
	unsigned int i, j, n, k, money, amount, min = 0;
	char a[5000][2], tmp[2];

	while(fscanf(in, "%d%d", &amount, &n) != EOF){
		money = 0;
		min = 0;
		for(i=0; i<n; i++)
			fscanf(in, "%d%d", &(a[i][0]), &(a[i][1]));
		for(i=0; i<n; i++){
			min = i;
			for(j=i; j<n; j++)
				if(a[j][0] < a[min][0])
					min = j;
			tmp[0] = a[i][0];
			tmp[1] = a[i][1];
			a[i][0] = a[min][0];
			a[i][1] = a[min][1];
			a[min][0] = tmp[0];
			a[min][1] = tmp[1];
		}
		while(amount > 0){
			for(i=0; i<n; i++){
				if(amount >= a[i][1]){
					money += a[i][1]*a[i][0];
					amount -= a[i][1];
				}
				else{
					money += a[i][0]*amount;
					amount = 0;
				}
			}
			fprintf(out, "%d\n", money);
		}
	}
	return 0;
}
//###END
