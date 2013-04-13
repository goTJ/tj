/*   @JUDGE_ID:   10387ZJ   160   C */
#include<stdio.h>

char prime[40];
char pl = 1;
int tab[40];

void pprime(int n)
{
	int i;

	if(!(n%2))
		return;
	for(i=3; i*i<=n; i+=2)
		if(!(n%i))
			return;
	prime[pl] = n;
	pl++;
}

void judge(int n)
{
	int i;

	for(i=0; n!=1&&i<pl; i++){
		while(!(n%prime[i])){
			n /= prime[i];
			tab[i]++;
		}
	}
}

int main(void)
{
	int i, n, l;

	for(i=2; i<100; i++)
		pprime(i);
	prime[0] = 2;
	while(scanf("%d", &n) != EOF){
		if(!n)
			exit(0);
		memset(tab, 0, sizeof(tab));
		for(i=2; i<=n; i++)
			judge(i);
		printf("%3d! =", n);
		for(l=39; !tab[l]; l--);
		for(i=0; i<=l; i++){
			if(!(i%15) && i!=0)
				printf("\n      ");
			printf("%3d", tab[i]);
		}
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
