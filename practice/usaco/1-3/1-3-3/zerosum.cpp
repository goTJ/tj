//### PROGRAM
/*
ID: flyherm003
PROG: zerosum
*/
#include<iostream>
#include<fstream>
#include<iomanip>
#include<stdlib.h>

ifstream fin("zerosum.in");
ofstream fout("zerosum.out");

int maxDepth;

void search(int depth, int val, int sum, char way[])
{
	if(depth > maxDepth){
		sum += val;
		if(!sum)
			fout << way << endl;
	}
	else{
		way[(depth-2)*2+1] = ' ';
		search(depth+1, val/abs(val)*(abs(val*10)+depth), sum, way);

		way[(depth-2)*2+1] = '+';
		search(depth+1, depth, sum+val, way);

		way[(depth-2)*2+1] = '-';
		search(depth+1, depth*-1, sum+val, way);
	}
}

int main(void)
{
	char way[25];

	fin >> maxDepth;
	for(int i=0; i<maxDepth; i++)
		way[i*2] = static_cast<char>(i+1+'0');
	way[(maxDepth-1)*2+1] = '\0';
	search(2, 1, 0, way);
	return 0;
}
//### END
