//### PROGRAM
/*
ID: flyherm003
PROG: fence4
*/
#include<iostream>
#include<fstream>
#include<math.h>
#include<stdlib.h>
#include<float.h>
#define MAX 200
#define PI acos(-1)

ifstream fin("fence4.in");
ofstream fout("fence4.out");

struct Coor{
	int x;
	int y;
};

Coor ob;

struct Vertex{
	Coor p;		// pos
	Coor v;		// vector
	bool see;
};

Vertex node[MAX];
int lim;

struct Sight{
	double arg;	// angle
	bool see;
};

Sight sight[MAX*3];
int slim=0;

int comp(const void *p, const void *q)
{
	if(((Sight*)p)->arg > ((Sight*)q)->arg)
		return 1;
	else if(((Sight*)p)->arg < ((Sight*)q)->arg)
		return -1;
	else
		return 0;
}

inline void swap(double &a, double &b)
{
	double tmp=a;

	a = b;
	b = tmp;
}

inline bool equal(double a, double b)
{
	if(a>b-0.000001 && a<b+0.000001)
		return true;
	return false;
}

inline int cross(int ax, int ay, int bx, int by)	// lazy
{
	return ax*by - ay*bx;
}

inline int dot(int ax, int ay, int bx, int by)
{
	return ax*bx + ay*by;
}

inline bool isIntersect(Vertex a, Vertex b)
{
	if(a.v.x*b.v.y == a.v.y*b.v.x){
		if((a.p.x-b.p.x)*a.v.y == (a.p.y-b.p.y)*a.v.x){
			int min=(b.p.x<b.p.x+b.v.x) ? b.p.x : b.p.x+b.v.x;
			int max=(b.p.x>b.p.x+b.v.x) ? b.p.x : b.p.x+b.v.x;
			if((a.p.x>min&&a.p.x<max) || (a.p.x+a.v.x<max&&a.p.x+a.v.x>min))
				return true;
		}
		return false;
	}
	else{
		if(cross(a.v.x, a.v.y, b.p.x-a.p.x, b.p.y-a.p.y) * cross(a.v.x, a.v.y, b.p.x+b.v.x-a.p.x, b.p.y+b.v.y-a.p.y) >= 0)
			return false;
		if(cross(b.v.x, b.v.y, a.p.x-b.p.x, a.p.y-b.p.y) * cross(b.v.x, b.v.y, a.p.x+a.v.x-b.p.x, a.p.y+a.v.y-b.p.y) >= 0)
			return false;
		return true;
	}
}

inline double trueAngle(double angle)
{
	while(angle < 0.0)
		angle += 2*PI;
	while(angle > 2*PI)
		angle -= 2*PI;
	return angle;
}

inline double countAngle(int x, int y)
{
	return trueAngle( atan2(y, x) );
}

void setSight(int x, int y)
{
	sight[slim].arg = countAngle(x, y);
	sight[slim].see = true;
	for(int i=0; i<slim; i++){
		if(equal(sight[i].arg, sight[slim].arg))
			return;
	}
	slim++;
}

bool input(void)
{
	fin >> lim;
	fin >> ob.x >> ob.y;
	fin >> node[0].p.x >> node[0].p.y;
	for(int i=0; i<lim; i++){
		int next=(i+1)%lim;
		if(next)
			fin >> node[next].p.x >> node[next].p.y;
		node[i].v.x = node[next].p.x-node[i].p.x;
		node[i].v.y = node[next].p.y-node[i].p.y;
		node[i].see = false;
		for(int j=0; j<i; j++){
			if(isIntersect(node[i], node[j])){
				fout << "NOFENCE" << endl;
				return false;
			}
		}
		setSight(node[i].p.x-ob.x, node[i].p.y-ob.y);
		if(dot(node[i].v.y*-1, node[i].v.x, node[i].p.x-ob.x, node[i].p.y-ob.y) > 0)
			setSight(node[i].v.y*-1, node[i].v.x);
		else
			setSight(node[i].v.y, node[i].v.x*-1);
	}
	qsort(sight, slim, sizeof(Sight), comp);
	return true;
}

inline double countDis(int a)
{
	double dis;
	double begin=countAngle(node[a].p.x-ob.x, node[a].p.y-ob.y);
	double end=countAngle(node[a].p.x+node[a].v.x-ob.x, node[a].p.y+node[a].v.y-ob.y);
	double middle;
	double maxCos=0.0, tmp;
	int i;
	bool last=false;

	dis = fabs(cross(node[a].v.x, node[a].v.y, ob.x-node[a].p.x, ob.y-node[a].p.y))/sqrt(node[a].v.x*node[a].v.x+node[a].v.y*node[a].v.y);
	if(dot(node[a].v.y*-1, node[a].v.x, node[a].p.x-ob.x, node[a].p.y-ob.y) > 0)
		middle = countAngle(node[a].v.y*-1, node[a].v.x);
	else
		middle = countAngle(node[a].v.y, node[a].v.x*-1);

	if(trueAngle(end-begin) > PI)
		swap(begin, end);
	for(i=0; sight[i].arg<begin; i++)
		;
	for(; !equal(sight[i].arg, end); i=(i+1)%slim){
		if(last || sight[i].see){
			if((tmp=cos(sight[i].arg-middle)) > maxCos)
				maxCos = tmp;
		}
		last = sight[i].see;
	}
	if(last){
		if((tmp=cos(sight[i].arg-middle)) > maxCos)
			maxCos = tmp;
	}
	if(equal(maxCos, 0.0))
		return DBL_MAX;
	else
		return dis/maxCos;
}

void coverSight(int a)
{
	double begin=countAngle(node[a].p.x-ob.x, node[a].p.y-ob.y);
	double end=countAngle(node[a].p.x+node[a].v.x-ob.x, node[a].p.y+node[a].v.y-ob.y);
	int i;

	if(trueAngle(end-begin) > PI)
		swap(begin, end);
	for(i=0; sight[i].arg<begin; i++)
		;
	for(; !equal(sight[i].arg, end); i=(i+1)%slim)
		sight[i].see = false;
}

int main(void)
{
	if(input()){
		double minDis=0.0;
		int index;
		int sum=0;

		while(minDis != DBL_MAX){
			double tmp;
			minDis = DBL_MAX;
			for(int i=0; i<lim; i++){
				if((tmp=countDis(i)) < minDis-0.000001){
					minDis = tmp;
					index = i;
				}
				else if(equal(tmp, minDis)){
					double dis=fabs(cross(node[i].v.x, node[i].v.y, ob.x-node[i].p.x, ob.y-node[i].p.y))/sqrt(node[i].v.x*node[i].v.x+node[i].v.y*node[i].v.y);
					double diss=fabs(cross(node[index].v.x, node[index].v.y, ob.x-node[index].p.x, ob.y-node[index].p.y))/sqrt(node[index].v.x*node[index].v.x+node[index].v.y*node[index].v.y);
					cout << "\tlala: " << i << " " << dis << endl;
					cout << "\t      " << index << " " << diss << endl;
					if(dis > diss){
						index = i;
					}
				}
				if(i<30 && i>20)
					cout << i << ": " << tmp << "\t" << minDis<< endl;
			}
			cout << "min: " << index << endl;
			node[index].see = true;
			sum++;
			coverSight(index);
		}
		sum--;
		fout << sum << endl;
		for(int i=0; i<lim-1; i++){
			if(i == lim-2){
				if(node[i+1].see)
					fout << node[i+1].p.x+node[i+1].v.x << " " << node[i+1].p.y+node[i+1].v.y << " " << node[i+1].p.x << " " << node[i+1].p.y << endl;
			}
			if(node[i].see)
				fout << node[i].p.x << " " << node[i].p.y << " " << node[i].p.x+node[i].v.x << " " << node[i].p.y+node[i].v.y << endl;
		}
	}
	return 0;
}
//### END
