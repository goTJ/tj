#include<stdio.h>
#include<math.h>

#define PI acos(-1)

struct Coor{
	double x, y;
};

double determinant(double a1, double b1, double a2, double b2)
{
	return a1*b2 - a2*b1;
}

double distance(const Coor &a, const Coor &b)
{
	return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

void swap(double &a, double &b)
{
	double t;

	t = a;
	a = b;
	b = t;
}

bool equal(double a, double b)
{
	if((a-b) < 0.0001 && (a-b) > -0.0001)
		return true;
	else
		return false;
}

double get_gcd(double a, double b)
{
	double t;

	while(!equal(b, 0.0)){
		t = b;
		b = fmod(a, b);
		a = t;
	}
	return a;
}

int main(void)
{
	int round, times;
	Coor n[3], v[2], center;
	double c[2];
	int i, j;
	double t[3], degree[3];
	double radius;
	int minp;
	double gcd;

	scanf("%d", &times);
	for(round=0; round<times; round++){
		for(i=0; i<3; i++)
			scanf("%lf%lf", &n[i].x, &n[i].y);
		// 求 v[i].x * x + v[i].y * y = c[i]  中垂線方程式
		for(i=0; i<2; i++){
			v[i].x = n[i].x-n[i+1].x;
			v[i].y = n[i].y-n[i+1].y;
			c[i] = v[i].x*((n[i].x+n[i+1].x)/2) + v[i].y*((n[i].y+n[i+1].y)/2);
		}
		// 克拉瑪求圓心
		center.x = determinant(c[0], v[0].y, c[1], v[1].y) / determinant(v[0].x, v[0].y, v[1].x, v[1].y);
		center.y = determinant(v[0].x, c[0], v[1].x, c[1]) / determinant(v[0].x, v[0].y, v[1].x, v[1].y);
//		printf("%lf %lf\n", center.x, center.y);
		radius = distance(n[0], center);
		// 平移伸縮座標使得頂點落於單位圓上
		for(i=0; i<3; i++){
			n[i].x = (n[i].x-center.x)/radius;
			n[i].y = (n[i].y-center.y)/radius;
		}
		// 求三頂點的角度
		for(i=0; i<3; i++){
			t[i] = atan2(n[i].y, n[i].x);
			while(t[i] < 0.000000000)
				t[i] += 2*PI;
		}
		// 由小到大排序三頂點角度
		for(i=0; i<3; i++){
			minp = i;
			for(j=i+1; j<3; j++){
				if(t[j] < t[minp])
					minp = j;
			}
			swap(t[i], t[minp]);
		}
		// 設定夾角
		degree[0] = t[1]-t[0];
		degree[1] = t[2]-t[1];
		degree[2] = 2*PI - degree[0] - degree[1];
		// 求夾角的最大公因式
		gcd = get_gcd(get_gcd(degree[0], degree[1]), degree[2]);
		printf("%.0lf\n", 2*PI/gcd);
	}
	return 0;
}
