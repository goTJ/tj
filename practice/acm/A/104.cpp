#include<stdio.h>
#define NUM 20

struct Node{
	double val;
	int from;
};

int main()
{
	double rate[NUM][NUM];
	int num;

	while(scanf("%d", &num) == 1){
		// input & initialize
		for(int i=0; i<num; i++){
			for(int j=0; j<num; j++){
				if(i != j)
					scanf("%lf", &rate[i][j]);
			}
		}
		// judge
		Node tab[NUM][NUM];
		int min=num;
		int path[NUM];
		for(int s=0; s<num; s++){
			for(int i=0; i<num; i++)
				tab[0][i].val = 0.0;
			tab[0][s].val = 1.0;
			for(int times=1; times<min; times++){
				int i;
				for(i=0; i<num; i++){
					tab[times][i].val = tab[times-1][i].val;
					for(int j=0; j<num; j++){
						if(j!=i && tab[times-1][j].val*rate[j][i] > tab[times][i].val){
							tab[times][i].val = tab[times-1][j].val*rate[j][i];
							tab[times][i].from = j;
						}
					}
					if(tab[times][i].val*rate[i][s] > 1.009999)
						break;
				}
				if(i!=num && times<min){
					min = times;
					path[0] = s;
					for(; times>0; times--){
						path[times] = i;
						i = tab[times][i].from;
					}
					break;
				}
			}
		}
		// output
		if(min == num)
			printf("no arbitrage sequence exists\n");
		else{
			for(int i=0; i<=min; i++)
				printf("%d ", path[i]+1);
			printf("%d\n", path[0]+1);
		}
	}
	return 0;
}
