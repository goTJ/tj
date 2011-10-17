//### PROGRAM
/*
ID: flyherm003
PROG: friday
*/
#include<iostream>
#include<fstream>

ifstream fin("friday.in");
ofstream fout("friday.out");

int tab[2][12]={{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

int checkLeap(int n)
{
	if(!(n%4) && (n%100 || !(n%400)))
		return 1;
	return 0;
}

int main(void)
{
	int n;
	int isLeap;
	int sum[7]={0};
	int day=0;

	fin >> n;
	for(int i=0; i<n; i++){
		isLeap = checkLeap(1900+i);
		for(int j=0; j<12; j++){
			sum[day]++;
			day = (day+tab[isLeap][j])%7;
		}
	}
	fout << sum[0];
	for(int i=1; i<7; i++)
		fout << " " << sum[i];
	fout << endl;
	return 0;
}
//### END
