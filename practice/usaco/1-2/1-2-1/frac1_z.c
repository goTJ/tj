#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int n;

/* print the fractions of denominator <= n between n1/d1 and n2/d2 */
void genfrac(int n1, int d1, int n2, int d2)
{
	if(d1+d2 > n)	/* cut off recursion */
		return;
	genfrac(n1,d1, n1+n2,d1+d2);
	printf("%d/%d\n", n1+n2, d1+d2);
	genfrac(n1+n2,d1+d2, n2,d2);
}

int main(void)
{
	//freopen("frac1.in", "r", stdin);
	//freopen("frac1.out", "r", stdout);

	while(scanf("%d", &n) == 1){
		printf("0/1\n");
		genfrac(0,1, 1,1);
		printf("1/1\n");
	}
}

