/* @JUDGE_ID:   10319NX 524 C */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int prime[40];
int used[17];
int lim;

void initial(void)
{
	int i, j;

	memset(used, 0, sizeof(used));
	for(i=0; i<=lim*2; i++)
		prime[i] = 1;
	for(i=2; i<=lim*2; i++){
		if(prime[i]){
			for(j=i*2; j<=lim*2; j+=i)
				prime[j] = 0;
		}
	}
}

void output(int way[], int num)
{
	int i;

	printf("%d", way[0]);
	for(i=1; i<num; i++)
		printf(" %d", way[i]);
	printf("\n");
}

void gogogo(int way[], int depth)
{
	int i;

	if(depth == lim){
		if(prime[way[0]+way[depth-1]])
			output(way, depth);
	}
	else{
		for(i=1; i<=lim; i++){
			if(!used[i] && prime[i+way[depth-1]]){
				used[i] = 1;
				way[depth] = i;
				gogogo(way, depth+1);
				used[i] = 0;
			}
		}
	}
}

int main(void)
{
	int times=0;
	int way[17];

	while(scanf("%d", &lim) != EOF){
		if(times)
			printf("\n");
		initial();
		printf("Case %d:\n", times+1);
		way[0] = 1;
		used[1] = 1;
		gogogo(way, 1);
		times++;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
