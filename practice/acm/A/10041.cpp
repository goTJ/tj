#include<stdio.h>
#include<stdlib.h>

int comp(const void *p, const void *q)
{
	return *(int*)p-*(int*)q;
}

int main()
{
	int round;

	scanf("%d", &round);
	for(int times=0; times<round; times++){
		int n;
		int a[505];
		scanf("%d", &n);
		for(int i=0; i<n; i++)
			scanf("%d", &a[i]);
		qsort(a, n, sizeof(int), comp);
		int sum=0;
		for(int i=0; i<n; i++)
			sum += abs(a[i]-a[n/2]);
		printf("%d\n", sum);
	}
	return 0;
}
