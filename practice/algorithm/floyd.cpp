/*
   ������Jn m, �N��n�ӫ���, m�����|, �����s����1�}�l
   ����n��, �C�榳�T�ӭ�a b d, �N��a�Pb�⫰���۳s, ���׬�d
   ��X�����P���������̵u���|

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
