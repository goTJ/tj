//### PROGRAM
/*
ID: flyherm003
PROG: milk3
*/
#include<stdio.h>
#include<string.h>

char used[21][21][21];
int max[5];

void initial(void)
{
	memset(used, 0, sizeof(used));
}

void judge(int n[])
{
	int i, j, sub;

	if(used[n[0]][n[1]][n[2]] == 0){
		used[n[0]][n[1]][n[2]] = 1;
		for(i=0; i<3; i++){
			for(j=0; j<3; j++){
				if(j!=i && n[i]){
					sub = n[i];
					if(max[j]-n[j] < sub)
						sub = max[j]-n[j];
					n[i] -= sub;
					n[j] += sub;
					judge(n);
					n[i] += sub;
					n[j] -= sub;
				}
			}
		}
	}
}

int main(void)
{
	int n[5];
	int i, sum;
	freopen("milk3.in", "r", stdin);
	freopen("milk3.out", "w", stdout);

	while(scanf("%d%d%d", &(max[0]), &(max[1]), &(max[2])) != EOF){
		initial();
		n[0] = n[1] = 0;
		n[2] = max[2];
		judge(n);
		sum = max[2];
		for(i=0; i<=sum; i++){
			if(used[0][sum-i][i]){
				printf("%d", i);
				i++;
				break;
			}
		}
		for(; i<=sum; i++){
			if(used[0][sum-i][i])
				printf(" %d", i);
		}
		printf("\n");
	}
	return 0;
}
//### END
