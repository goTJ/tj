//### PROGRAM
/*
ID: flyherm003
PROG: concom
*/
#include<iostream>
#include<fstream>
#define MAX 100

ifstream fin("concom.in");
ofstream fout("concom.out");

int ori[MAX+1][MAX+1];
int sum[MAX+1][MAX+1];
bool control[MAX+1][MAX+1];

void initial(void)
{
	for(int i=1; i<=MAX; i++){
		for(int j=1; j<=MAX; j++){
			ori[i][j] = sum[i][j] = 0;
			control[i][j] = false;
		}
	}
	for(int i=1; i<=MAX; i++){
		ori[i][i] = sum[i][i] = 100;
		control[i][i] = true;
	}
}

void add(int a, int b)
{
	if(control[a][b] == false){
		control[a][b] = true;
		for(int i=1; i<=MAX; i++){
			sum[a][i] += ori[b][i];
		}
		for(int i=1; i<=MAX; i++){
			if(control[i][a] == true)
				add(i, b);
		}
	}
}

void own(int n)
{
	int flag=false;

	for(int i=1; i<=MAX; i++){
		if(sum[n][i]>50 && control[n][i]==false){
			add(n, i);
			i = 1;
			flag = true;
		}
	}
	if(flag == true){
		for(int i=1; i<=MAX; i++){
			if(control[i][n] == true)
				own(i);
		}
	}
}

int main(void)
{
	int n;

	initial();
	fin >> n;
	for(int i=0; i<n; i++){
		int a, b, p;
		fin >> a >> b >> p;
		ori[a][b] = sum[a][b] = p;
	}
	for(int i=1; i<=MAX; i++)
		own(i);
	for(int i=1; i<=MAX; i++){
		for(int j=1; j<=MAX; j++){
			if(control[i][j]==true && i!=j)
				fout << i << " " << j << endl;
		}
	}
	return 0;
}
//### END
