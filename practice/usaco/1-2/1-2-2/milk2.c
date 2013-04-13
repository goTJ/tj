//### PROGRAM
/*
ID: flyherm003
PROG: milk2
start: 
finish: 
*/
#include<stdio.h>
#include<stdlib.h>

struct fly{
	int begin;
	int end;
}time[5002];
int lim;

int comp(const void *p, const void *q)
{
	return ((struct fly *)p)->begin - ((struct fly *)q)->begin;
}

int find_continue(int *n, int *bmax)
{
	int i, min=time[*n].begin, max=time[*n].end;
	int tmp;

	for(i=*n; time[i].begin<=max && i<lim; i++){
		if(time[i].end > max)
			max = time[i].end;
	}
	if((tmp=time[i].begin-max) > *bmax)
		*bmax = tmp;
	*n = i;
	return max-min;
}

int main(void)
{
	int i, tmp, cmax, bmax;
	freopen("milk2.in",  "r",  stdin);
	freopen("milk2.out",  "w",  stdout);

	while(scanf("%d", &lim) != EOF){
		for(i=0; i<lim; i++)
			scanf("%d%d", &time[i].begin, &time[i].end);
		qsort(time, lim, sizeof(time[0]), comp);
		i = 0;
		cmax  = bmax = 0;
		while(i < lim)
			if((tmp=find_continue(&i, &bmax)) > cmax)
				cmax = tmp;
		printf("%d %d\n", cmax, bmax);
	}
	return 0;
}
//### END
