#include<stdio.h>
#include<math.h>

#define PI acos(-1)

int main(void)
{
	double a;

	while(scanf("%lf", &a) != EOF){
		printf("%.10lf ", cos(PI/6)*a);
		printf("%.10lf ", a*sin(PI/3)/(sin(PI/3)+1));
		printf("%.10lf ", a*sin(PI/3)/2);
		printf("%.10lf\n", sqrt(3)*a*(sqrt(84)-7)/10);
	}
	return 0;
}
