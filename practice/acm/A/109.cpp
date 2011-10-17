#include<cstdio>
#include<cmath>

struct Coor{
	int x,y;
};

class Vector{
public:
	double x,y;
	Vector(double, double);
	Vector(Coor&, Coor&);
	void set(double, double);
	void set(Coor&, Coor&);
};

Vector::Vector(double x, double y)
{
	set(x, y);
}

Vector::Vector(Coor &a, Coor &b)
{
	set(a, b);
}

void Vector::set(double x, double y)
{
	this->x = x/sqrt(x*x+y*y);
	this->y = y/sqrt(x*x+y*y);
}

void Vector::set(Coor &a, Coor &b)
{
	x = b.x-a.x;
	y = b.y-a.y;
}

class Country{
public:
	Coor point[105];
	double cx, cy;
	int lim;
	bool isBomb;
	void addPoint(Coor&);
	double countArea();
};

void Country::addPoint(Coor &a)
{
	point[lim] = a;
	lim++;
}

double Country::countArea()
{
	double sum=0.0;
	for(int i=0; i<lim; i++){
		sum += (point[i].x*point[i+1].y)-(point[i].y*point[i+1].x);
	}
	return sum/2;
}

bool equal(double a, double b)
{
	return fabs(a-b) < 0.0001;
}

double distance(Coor a, Coor b)
{
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

bool isIn(Coor p, Country c)
{
	for(int i=0; i<c.lim; i++){
		Vector tv(c.point[i], c.point[i+1]);
		Coor tc=c.point[i];
		if((tv.y*p.x-tv.x*p.y-tv.y*tc.x+tv.x*tc.y)*(tv.y*c.cx-tv.x*c.cy-tv.y*tc.x+tv.x*tc.y) < 0.0000001)
			return false;
	}
	return true;
}

int main()
{
	Country country[22];
	int cLim;
	int n;
	// sketch country
	for(cLim=0; scanf("%d", &n)==1; cLim++){
		if(n == -1)
			break;
		country[cLim].lim = 0;
		country[cLim].isBomb = false;
		Coor buf[105];
		bool used[105];
		int minp=-1;
		for(int i=0; i<n; i++){
			scanf("%d%d", &(buf[i].x), &(buf[i].y));
			for(int j=0; j<i; j++){
				if(buf[j].x == buf[i].x && buf[j].y == buf[i].y){
					i--;
					n--;
					break;
				}
			}
			used[i] = false;
			if(minp == -1 || buf[i].x < buf[minp].x)
				minp = i;
		}
		// find
		Coor origin=buf[minp];
		country[cLim].addPoint(buf[minp]);
		used[minp] = true;
		int start = minp;
		Vector base(0.0, -1.0);
		for(int i=1; i<=n; i++){
			minp = -1;
			double minAngle;
			double Len;
			for(int j=0; j<n; j++){
				if(!used[j]){
					Vector t((double)buf[j].x-origin.x, (double)buf[j].y-origin.y);
					double tmpAngle=acos(t.x*base.x+t.y*base.y);
					double tmpLen=distance(origin, buf[j]);
					if(minp == -1 || tmpAngle<minAngle || (equal(tmpAngle, minAngle) && tmpLen > Len)){
						minp = j;
						minAngle = tmpAngle;
						Len = tmpLen;
					}
				}
			}
			country[cLim].addPoint(buf[minp]);
			if(minp == start)
				break;
			origin = buf[minp];
			used[minp] = true;
			base.set(country[cLim].point[country[cLim].lim-1].x-country[cLim].point[country[cLim].lim-2].x
					, country[cLim].point[country[cLim].lim-1].y-country[cLim].point[country[cLim].lim-2].y);
			if(i == 2)
				used[start] = false;
		}
		country[cLim].lim--;
		country[cLim].cx = country[cLim].point[0].x;
		country[cLim].cy = country[cLim].point[0].y;
		for(int i=1; i<country[cLim].lim; i++){
			country[cLim].cx += country[cLim].point[i].x;
			country[cLim].cy += country[cLim].point[i].y;
		}
		country[cLim].cx /= country[cLim].lim;
		country[cLim].cy /= country[cLim].lim;
////		printf("%d: center(%lf, %lf)\n", cLim, country[cLim].cx, country[cLim].cy);
////		for(int i=0; i<country[cLim].lim; i++)
////			printf("%d %d\n", country[cLim].point[i].x, country[cLim].point[i].y);
////		printf("\n");
	}
	Coor bomb;
	double sum=0.0;
	while(scanf("%d%d", &(bomb.x), &(bomb.y)) == 2){
////		printf("bomb: %d %d\n", bomb.x, bomb.y);
		for(int i=0; i<cLim; i++){
			if(!country[i].isBomb && isIn(bomb, country[i])){
				sum += country[i].countArea();
				country[i].isBomb = true;
				break;
			}
		}
	}
////	for(int i=0; i<cLim; i++){
////		if(country[i].isBomb)
////			printf("%d ", i);
////	}
////	printf("\n");
	printf("%.2lf\n", sum);
	return 0;
}
