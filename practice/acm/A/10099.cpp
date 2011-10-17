/* @JUDGE_ID:   10319NX 10099 C++ "¦ó¦Ì¯S^^" */
#include<iostream>

int dis[105][105];
int N;

void initial(void)
{
	int i, j;

	for(i=1; i<=N; i++)
		for(j=1; j<=N; j++)
			dis[i][j] = 0;
}

int min(int a, int b)
{
	return (a<b) ? a : b;
}

void floyd(void)
{
	int i, j, k;
	int t;

	for(k=1; k<=N; k++){
		for(i=1; i<=N; i++){
			for(j=1; j<=N; j++){
				if((t=min(dis[i][k], dis[k][j]))>dis[i][j])
					dis[i][j] = t;
			}
		}
	}
}

int main(void)
{
	int n, i;
	int a, b, c;
	int max;
	int times=1;

	while(cin >> N >> n){
		if(!N && !n)
			break;
		initial();
		for(i=0; i<n; i++){
			cin >> a >> b >> c;
			dis[a][b] += c;
			dis[b][a] += c;
		}
		cin >> a >> b >> c;
		floyd();
		max = dis[a][b];
		max--;
		cout << "Scenario #" << times << endl;
		cout << "Minimum Number of Trips = " << (c+max-1)/max << endl << endl;
		times++;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
