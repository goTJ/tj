//### PROGRAM
/*
ID: flyherm003
PROG: transform
*/
#include<iostream>
#include<fstream>

ifstream fin("transform.in");
ofstream fout("transform.out");

int lim;
bool ok[8];
int tab[8]={6, 5, 4, 2, 5, 3, 1, 5};
int ansH[10]={0}, ansV[10]={0};

void initial(void)
{
	for(int i=1; i<=7; i++)
		ok[i] = false;
	ok[7] = true;
}

void judge(int h[], int hb, int ha, int v[], int vb, int va, int num)
{
	for(int i=hb,j=0; j<lim; i+=ha,j++){
		if(ansH[j] != h[i])
			return;
	}
	for(int i=vb,j=0; j<lim; i+=va,j++){
		if(ansV[j] != v[i])
			return;
	}
	ok[tab[num]] = true;
}

void vv(int h[], int hb, int ha, int v[], int num)
{
	judge(h, hb, ha, v, 0, 1, num);
	judge(h, hb, ha, v, lim-1, -1, num+1);
}

void hh(int h[], int v[], int num)
{
	vv(h, 0, 1, v, num);
	vv(h, lim-1, -1, v, num+2);
}

void change(int h[], int v[], int num)
{
	hh(h, v, num);
	hh(v, h, num+4);
}

void input(int h[], int v[])
{
	for(int i=0; i<lim; i++){
		char buf[11];
		fin >> buf;
		for(int j=0; j<lim; j++){
			if(buf[j] == '@'){
				h[j]++;
				v[i]++;
			}
		}
	}
}

int main(void)
{
	int h[10]={0}, v[10]={0};

	initial();
	fin >> lim;
	input(h, v);
	input(ansH, ansV);
	change(h, v, 0);
	for(int i=1; i<=7; i++){
		if(ok[i]){
			fout << i << endl;
			break;
		}
	}
	return 0;
}
//### END
