//### PROGRAM
/*
ID: flyherm003
PROG: camelot
*/
#include<iostream>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#define ROW 40
#define COL 26

ifstream fin("camelot.in");
ofstream fout("camelot.out");

struct coor{
	int r;
	int c;
};

struct node{
	int dis;
	int sum;
	int kDis;
	int kingDis;
	int knum;
	int used;
};
node map[ROW][COL];

int row, col;
int knightWay[8][2] = { 2,1, 2,-1, -2,1, -2,-1, 1,2, 1,-2, -1,2, -1,-2 };
coor kingPos;

void display(void)
{
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			if(map[i][j].dis==-1)
				cout << " * ";
			else
				cout << setw(2) << map[i][j].dis << " ";
		}
		cout << endl;
	}
	cout << endl;
}

int read(coor &a)
{
	char buf[5];

	if(fin >> buf >> a.r){
		a.c = buf[0]-'A';
		a.r--;
		return 1;
	}
	return 0;
}

inline int max(int a, int b)
{
	return a>b ? a : b;
}

void initial(void)
{
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			map[i][j].dis = -1;
			map[i][j].used = 1;
			map[i][j].sum = 0;
			map[i][j].knum = 0;
			map[i][j].kingDis = map[i][j].kDis = max(abs(i-kingPos.r), abs(j-kingPos.c));
		}
	}
}

inline int check(coor &a)
{
	if(a.r>=0 && a.r<row && a.c>=0 && a.c<col)
		return 1;
	return 0;
}

int dfsId(coor pos, int depth, int maxDepth, int used)
{
	register coor t;

//	cout << "dfsId: " << pos.r << " " << pos.c << " - " << depth << " " << used << endl;
	map[pos.r][pos.c].used *= -1;
	if(depth == maxDepth){
		map[pos.r][pos.c].dis = depth;
		map[pos.r][pos.c].sum += depth;
		map[pos.r][pos.c].used = used*-1;
		map[pos.r][pos.c].knum++;
		for(int i=0; i<8; i++){
			t.r = pos.r+knightWay[i][0];
			t.c = pos.c+knightWay[i][1];
			if(check(t) && map[t.r][t.c].dis!=-1 && map[t.r][t.c].kDis<map[pos.r][pos.c].kDis)
				map[pos.r][pos.c].kDis = map[t.r][t.c].kDis;
		}
		if(map[pos.r][pos.c].kDis < map[pos.r][pos.c].kingDis)
			map[pos.r][pos.c].kingDis = map[pos.r][pos.c].kDis;
		return true;
	}
	else{
		bool flag=false;

		for(int i=0; i<8; i++){
			t.r = pos.r+knightWay[i][0];
			t.c = pos.c+knightWay[i][1];
			if(check(t) && ((depth+1!=maxDepth&&map[t.r][t.c].dis>depth&&map[t.r][t.c].used==used)||(depth+1==maxDepth&&map[t.r][t.c].dis==-1)))
				flag = flag | dfsId(t, depth+1, maxDepth, used);
		}
		return flag;
	}
}

int main(void)
{
	coor knight;
	int min=10000000;
	int knum=0;
	int used=-1;

	fin >> row >> col;
	read(kingPos);
	initial();
	while(read(knight)){
		int maxDepth=0;

//		cout << knight.r << " " << knight.c << endl;
		while(dfsId(knight, 0, maxDepth, used)){
			maxDepth++;
			used *= -1;
		}
//		display();
		for(int i=0; i<row; i++)
			for(int j=0; j<col; j++){
				map[i][j].dis = -1;
				map[i][j].kDis = max(abs(i-kingPos.r), abs(j-kingPos.c));
			}
		knum++;
	}
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			if(map[i][j].knum==knum && map[i][j].sum+map[i][j].kingDis<min)
				min = map[i][j].sum+map[i][j].kingDis;
		}
	}
//	cout << "min: ";
	fout << min << endl;
	return 0;
}
//### END
