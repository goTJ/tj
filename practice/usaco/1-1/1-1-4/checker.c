//### PROGRAM
/*
ID: flyherm003
PROG: checker
*/
#include<stdio.h>

enum { MAX = 13 , MMAX = 26};

int n;
int flyhermit;
int fly928[MAX];

void output(int *way)
{
	int i;

	for(i=0; i<n-1; i++)
		printf("%d ", (way[i])+1);
	printf("%d\n", (way[i]+1));
}

int check(int rt[], int mt[], int lt[], int i, int deep)
{
	if(!mt[i])
		return 0;
	else if(!rt[i+deep])
		return 0;
	else if(!lt[i+n-1-deep])
		return 0;
	else
		return 1;
}

void judge(int deep, int way[], int lt[], int rt[], int mt[], int *find)
{
	int i, nn = n;
	int j;

	if(!deep)
		nn = (n+1)/2;
	if(deep == n){
		if((*find) == 1){
			for(i=0; i<n; i++)
				fly928[i] = n - way[i] -1;
		}
		if(way[0] < n/2)
			(*find)++;
		else
			flyhermit++;
		if((*find)<4)
			output(way);
	}
	else{
		for(i=0; i<nn; i++)
			if(check(rt, mt, lt, i, deep)){
				way[deep] = i;
				lt[i+n-1-deep] = rt[i+deep] = mt[i] = 0;
				judge(deep+1, way, lt, rt, mt, find);
				lt[i+n-1-deep] = rt[i+deep] = mt[i] = 1;
			}
	}
}

int main(void)
{
	int i, j;
	int way[MAX];
	int lt[MMAX+1], rt[MMAX+1], mt[MAX];
	int find;

	freopen("checker.in", "r", stdin);
	freopen("checker.out", "w", stdout);

	while(scanf("%d", &n) != EOF){
		find = flyhermit = 0;
		for(i=0; i<n*2; i++)
			lt[i] = mt[i] = rt[i] = 1;
		judge(0, way, lt, rt, mt, &find);
		if(find<3)
			output(fly928);
		printf("%d\n", find*2+flyhermit);
	}
	return 0;
}
//### END
