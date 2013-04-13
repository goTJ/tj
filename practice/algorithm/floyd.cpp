/*
   首先輸入n m, 代表有n個城市, m條路徑, 城市編號由1開始
   接著n行, 每行有三個值a b d, 代表a與b兩城市相連, 長度為d
   輸出城市與城市間的最短路徑

   algorithm: floyd & warshall
   time complexity: O(n^3)
*/
#include<iostream>
#include<iomanip>
#define MAX 100
#define MAX_DIS 100000

int dis[MAX][MAX];
int lim;

void initial(void)
{
	for(int i=0; i<lim; i++)
		for(int j=0; j<lim; j++)
			dis[i][j] = MAX_DIS;
}

void floyd(void)
{
	for(int k=0; k<lim; k++){
		for(int i=0; i<lim; i++){
			for(int j=0; j<lim; j++){
				if(dis[i][k]+dis[k][j] < dis[i][j])
					dis[i][j] = dis[i][k]+dis[k][j];
			}
		}
	}
}

int main(void)
{
	int n;

	// initial
	cin >> lim >> n;
	initial();
	// input
	for(int i=0; i<n; i++){
		int a, b, d;
		cin >> a >> b >> d;
		a--; b--;
		if(d < dis[a][b]){
			dis[a][b] = d;
			dis[b][a] = d;
		}
	}
	// judge
	floyd();
	// output
	for(int i=0; i<lim; i++){
		for(int j=0; j<lim; j++){
			if(dis[i][j] == MAX_DIS)
				cout << setw(3) << "x" << " ";
			else
				cout << setw(3) << dis[i][j] << " ";
		}
		cout << endl;
	}
	return 0;
}
