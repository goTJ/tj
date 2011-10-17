#include<stdio.h>
#include<math.h>

#define PI acos(-1)

int main()
{
	double D, V;

	while(scanf("%lf%lf", &D, &V) && D && V){
		printf("%.3lf\n", pow(pow(D, 3.0)-V*6.0/PI, 1.0/3.0));
	}
	return 0;
}
