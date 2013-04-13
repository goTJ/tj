/*
   ������Jn m, �N��n�ӫ���, m�����|, �����s����1�}�l
   ����n��, �C�榳�T�ӭ�a b d, �N��a�Pb�⫰���۳s, ���׬�d
   �̫��J�Ʀrp, �n�A��X�s��p�����P��L�������̵u���|.

   algorithm: dijkstra
   time complexity: O(n^2)
*/
#include<iostream>
#define MAX_DIS 100000
#define MAX 100

int dis[MAX][MAX];
int from[MAX];
int lim;

void initial(void)
{
	int i, j;

	for(i=0; i<lim; i++){
		for(j=0; j<lim; j++)
			dis[i][j] = MAX_DIS;
		dis[i][i] = 0;
	}
}

void dijkstra(int src)
{
	int i, j;
	int min, dest;
	int used[MAX]={0};

	for(i=0; i<MAX; i++)
		from[i] = src;
	used[src] = 1;
	for(i=1; i<lim; i++){
		dest = -1;
		min = MAX_DIS;
		for(j=0; j<lim; j++){
			if(!used[j] && dis[src][j]<min){
				min = dis[src][j];
				dest = j;
			}
		}
		if(dest == -1)
			return;
		used[dest] = 1;
		for(j=0; j<lim; j++){
			if(dis[src][j] > dis[src][dest]+dis[dest][j]){
				dis[src][j] = dis[src][dest]+dis[dest][j];
				from[j] = dest;
			}
		}
	}
}

void output(int src, int n)
{
	if(src != n)
		output(src, from[n]);
	cout << n+1 << " ";
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
	cin >> n;
	n--;
	dijkstra(n);
	// output
	for(int i=0; i<lim; i++){
		if(dis[n][i] == MAX_DIS)
			cout << i+1 << ": Not matching" << endl;
		else{
			cout << i+1 << ": ";
			output(n, i);
			cout << "-> " << dis[n][i] << endl;
		}
	}
	return 0;
}
