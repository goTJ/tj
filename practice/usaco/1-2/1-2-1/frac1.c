//### PROGRAM
/*
ID: flyherm003
PROG: frac1
*/
#include<stdio.h>

int num;
float tab[162];

int done(void)
{
	int i;

	for(i=1; i<=num; i++)
		if((int)tab[i]!=i)
			return 0;
	return 1;
}

int prime(int a, int b)
{
	int t=1;

	while(t){
		t = a%b;
		a = b;
		b = t;
	}
	return a;
}

int main(void)
{
	int i, min;
	float minn, t;
	//freopen("frac1.in",  "r",  stdin);
	//freopen("frac1.out",  "w",  stdout);
	
	while(scanf("%d", &num) != EOF){
		for(i=1; i<=num; i++)
			tab[i] = 1;
		printf("0/1\n");
		while(!done()){
			minn = tab[1]/1;
			min = 1;
			for(i=2; i<=num; i++){
				while(prime((int)tab[i], i)!=1 && (int)tab[i]<i)
					tab[i]++;
				if((t=tab[i]/i) < minn){
					min = i;
					minn = t;
				}
			}
			printf("%.0f/%d\n", tab[min], min);
			tab[min]++;
		}
		printf("1/1\n");
	}
	return 0;
}
//### END
