//### PROGRAM
/*
ID: flyherm003
PROG: numtri
*/
#include<stdio.h>

unsigned int num;

int min(unsigned int last[], int n, int nn)
{
	if((n-1) < 0)
		return last[n];
	if(n == nn)
		return last[n-1];
	if(last[n-1] > last[n])
		return last[n-1];
	else
		return last[n];
}

void judge(int n, unsigned int neww[], unsigned int last[])
{
	int i;

	for(i=0; i<=n; i++)
		neww[i] += min(last, i, n);
	for(i=0; i<=n; i++)
		last[i] = neww[i];
}

int main(void)
{
	int i, j;
	unsigned int min, last[1000], neww[1000];
	freopen("numtri.in",  "r",  stdin);
	freopen("numtri.out",  "w",  stdout);

	while(scanf("%u", &num) != EOF){
		scanf("%u", &(last[0]));
		for(i=1; i<num; i++){
			for(j=0; j<=i; j++)
				scanf("%d", &(neww[j]));
			judge(i, neww, last);
		}
		min = last[0];
		for(i=0; i<num; i++)
			if(last[i]>min)
				min = last[i];
		printf("%u\n", min);
	}
	return 0;
}
//### END
