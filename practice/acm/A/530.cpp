#include<cstdio>

int main()
{
	long long n,k;

	while(scanf("%lld%lld", &n, &k) == 2){
		if(!n && !k)
			break;

		long long sum=1;
		if(k > n/2)
			k = n-k;
		for(int i=0; i<k; i++){
			sum *= n-i;
			sum /= i+1;
		}
		printf("%lld\n", sum);
	}
	return 0;
}
