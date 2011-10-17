//### PROGRAM
/*
ID: flyherm003
PROG: pprime
*/
#include<stdio.h>

unsigned int isprime(unsigned int n)
{
	unsigned int i;

	if(!(n%2))
		return 0;
	else
		for(i=3; i*i<=n; i+=2)
			if(!(n%i))
				return 0;
	return 1;
}

unsigned int cir(unsigned int num)
{
	unsigned int n=0;

	do{
		n = num%10+n*10;
		printf("%d ", num);
	}while(num/=10);
	//printf("go cir %d -> %d\n", num, n);
	return n;
}

unsigned int many(unsigned int num)
{
	unsigned int n = 1;

	//printf("go num %d", num);
	while(num/n)
		n *= 10;
	//printf("  %d\n", n);
	return n;
}

unsigned int maknum(unsigned int out, unsigned int in)
{
	unsigned int sum;

	//printf("go maknum %d %d\n", out, in);
	if(out){
		//printf("hahah   ");
		if(in != 10)
			sum = many(out)*(out*10+in)+cir(out);	
		else
			sum = many(out)*out+cir(out);
		//printf("wuwuw   ");
	}
	else{
		if(in != 10)
			sum = in;
		else sum = 9;
	}
	//printf("   get: %d\n", sum);
	return sum;
}

int main(void)
{
	unsigned int min, max, middle, other, t;
	FILE *in = fopen("pprime.in", "r");
	FILE *out = fopen("pprime.out", "w");

	while( fscanf(in, "%d%d", &min, &max) != EOF){
		for(other=0; other<1000; other++){
			//printf("other: %d\n", other);
			for(middle=0; middle<10; middle++){
				//printf("middle: %d\n", middle);
				t = maknum(other, middle);
				if(t>max)
					exit(0);
				if(t>=min && isprime(t))
					fprintf(out, "%d\n", t);
			}
			t = maknum(other+1, 10);
			if(t>=max)
				exit(0);
			if(t>=min && isprime(t))
				fprintf(out, "%d\n", t);
		}
	}
	return 0;
}
//###END
