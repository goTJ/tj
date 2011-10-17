/*   @JUDGE_ID:   10387ZJ   136   C*/ 
#include<stdio.h>

enum { MAX = 100000 };
int point[3], a[MAX];
char num[3] = { 2,3,5 };

void check(int s)
{
	int i;

	for(i=0; i<3; i++)
		if(a[point[i]]*num[i] == s)
			point[i]++;
}

int min(int now)
{
	int i, s;

	s = a[point[0]]*num[0];
	for(i=1; i<3; i++)
		if(a[point[i]]*num[i]<s && a[point[i]]*num[i]>a[now])
			s = a[point[i]]*num[i];
	check(s);
	return s;
}

int main()
{
	int n=1500, i, t;

	a[0] = 1;
	memset( point, 0, sizeof(point));
	for(i=1; i<n; i++){
		a[i] = (t=min(i-1));
	}
	printf("The 1500'th ugly number is %d.\n", a[i-1]);
	return 0;
}
/* @END_OF_SOURCE_CODE */
