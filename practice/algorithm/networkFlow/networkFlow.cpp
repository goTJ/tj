/*
   無向圖的maxima network flow
   輸入格式:
   N	(有多少個node, 1<=N<=300)
   M	(有多少個edge, 1<=M<=1000)
   a b c	(a城和b城有一條capacity為c的edge, 1<=a,b<=N)
   ...
   s t	(從s城到t城)
   輸出格式:
   a single integer represents the maxima flow from S to T
*/
#include<stdio.h>
#include<string.h>

#define N 300

int num;
int map[N][N];

inline int min(const int &a, const int &b)
{
	return (a < b) ? a : b;
}

int find_path(const int &s, const int &t, int from[])
{
	register int capacity[N];
	register bool used[N];
	int i, j;
	int maxp;
	int temp;

	for(i=0; i<num; i++){
		capacity[i] = map[s][i];
		from[i] = s;
		used[i] = false;
	}
	used[s] = true;
	for(i=1; i<num; i++){
		maxp = -1;
		for(j=0; j<num; j++){
			if(!used[j] && (maxp==-1 || capacity[j] > capacity[maxp])){
				maxp = j;
			}
		}
		if(capacity[maxp] <= 0)
			break;
		if(maxp == t)
			break;
		used[maxp] = true;
		for(j=0; j<num; j++){
			temp = min(capacity[maxp], map[maxp][j]);
			if(temp > capacity[j]){
				capacity[j] = temp;
				from[j] = maxp;
			}
		}
	}
	return capacity[t];
}

void change(const int &flow, const int from[], const int &s, const int &t)
{
	int pos=t;

	while(pos != s){
		map[from[pos]][pos] -= flow;
		map[pos][from[pos]] += flow;
		pos = from[pos];
	}
}

int main()
{
	int n;
	int i, j;
	int a, b, c;
	int flow;
	int from[N];
	int sum;

	while(scanf("%d%d", &num, &n) != EOF){
		memset(map, 0, sizeof(map));
		sum = 0;
		for(i=0; i<n; i++){
			scanf("%d%d%d", &a, &b, &c);
			a--, b--;
			map[a][b] += c;
			map[b][a] += c;
		}
		scanf("%d%d", &a, &b);
		a--, b--;
		if(a == b){
			printf("Are you joking?\n");
			printf("I think its max-flow is infinite.\n");
		}
		else{
			while((flow=find_path(a, b, from)) > 0){
				sum += flow;
				change(flow, from, a, b);
			}
			printf("%d\n", sum);
		}
	}
	return 0;
}
