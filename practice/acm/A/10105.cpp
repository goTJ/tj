#include<cstdio>

inline double c(double n, double k)
{
	double sum=1;
	for(double i=0; i<k; i++)
		sum *= (n-i)/(i+1);
	return sum;
}

int main()
{
	double n, k;
	while(scanf("%lf%lf", &n, &k) == 2){
		double sum=1;
		for(double i=0; i<k; i++){
			double tmp;
			scanf("%lf", &tmp);
			sum *= c(n, tmp);
			n -= tmp;
		}
		printf("%.0lf\n", sum);
	}
	return 0;
}
