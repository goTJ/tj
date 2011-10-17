//### PROGRAM
/*
ID: flyherm003
PROG: heritage
*/
#include<iostream>
#include<fstream>
#include<string.h>

ifstream fin("heritage.in");
ofstream fout("heritage.out");

char seperate[30];

void judge(char *begin, char *end, int &pos)
{
	if(begin == end){
		fout << *begin;
		pos++;
	}
	else{
		char *tmp;
		for(tmp=begin; tmp<=end; tmp++){
			if(*tmp == seperate[pos])
				break;
		}
		pos++;
		if(tmp-1 >= begin)
			judge(begin, tmp-1, pos);
		if(tmp+1 <= end)
			judge(tmp+1, end, pos);
		pos--;
		judge(tmp, tmp, pos);
	}
}

int main(void)
{
	char in[30];
	int len, pos=0;

	fin >> in >> seperate;
	len = strlen(in);
	judge(in, in+len-1, pos);
	fout << endl;
	return 0;
}
//### END
