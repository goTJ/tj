//### PROGRAM
/*
id: flyherm003
prog: humble
*/
#include<stdio.h>
#include<stdlib.h>

unsigned int num[105];
unsigned int lim;
unsigned int K[100006];

int comp(const void *p, const void *q)
{
	return *(int *)p - *(int *)q;
}

void initial(void)
{
	K[0] = 1;
}

int main(void)
{
	unsigned int times;
	unsigned int first[105];
	unsigned int i, j, pos, lim_f;
	unsigned int min, t;
	int used[105];
////	freopen("humble.in", "r", stdin);
////	freopen("humble.out", "w", stdout);

	while(scanf("%u%u", &lim, &times) != EOF){
		initial();
		for(i=0; i<lim; i++)
			used[i] = 0;
		used[0] = 1;
		first[0] = 0;
		lim_f = 1;
		for(i=0; i<lim; i++)
			scanf("%u", &(num[i]));
		qsort(num, lim, sizeof(num[0]), comp);
		for(i=1; i<=times; i++){
			min = K[first[0]]*num[0];
			pos = 0;
			for(j=1; j<lim_f; j++){
				if(used[j] && (t=K[first[j]]*num[j])<min){
					min = t;
					pos = j;
				}
			}
			if(used[pos+1] == 0){
				used[pos+1] = 1;
				first[pos+1] = first[pos];
			}
			if(pos==0 || used[pos] || first[pos-1]-first[pos]>1)
				first[pos]++;
			else
				used[pos] =0;
			if(pos+1!=lim && pos+1==lim_f)
				lim_f++;
//			printf("%u: %u\n", i, min);
			if(min <= K[i-1]){
				i--;
				continue;
			}
			K[i] = min;
		}
		printf("%u\n", K[times]);
	}
	return 0;
}
//### END
