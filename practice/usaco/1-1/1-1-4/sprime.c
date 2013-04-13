//### PROGRAM
/*
ID: flyherm003
PROG: sprime
*/
#include<stdio.h>

unsigned int n;

int isprime(unsigned int num)
{
	int i;

	if(num==1)
		return 0;
	if(num==2)
		return 1;
	if(!(num%2))
		return 0;
	for(i=3; i*i<=num; i+=2)
		if(!(num%i))
			return 0;
	return 1;
}

void judge(int deep, unsigned int num)
{
	int i;

	if(deep == n){
		printf("%u\n", num);
	}
	else{
		for(i=1; i<10; i++)
			if(isprime(num*10+i))
				judge(deep+1, num*10+i);
	}
}

int main(void)
{
	FILE *in = fopen("sprime.in", "r");
	freopen("sprime.out", "w", stdout);

	while(fscanf(in, "%d", &n) != EOF){
		judge(0, 0);
	}
	return 0;
}
//### END
