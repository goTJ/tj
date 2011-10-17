//### PROGRAM
/*
id: flyherm003
prog: palsquare
*/
#include<stdio.h>
#include<string.h>

int base;
char sign[22];

void initial(void)
{
	int i;

	for(i=0; i<10; i++)
		sign[i] = i+'0';
	for(i=10; i<=20; i++)
		sign[i] = i-10+'A';
}

int add(int val[], int len)
{
	int i=0;

	val[0]++;
	while(val[i] == base){
		val[i] = 0;
		val[i+1]++;
		i++;
		if(i==len)
			return len+1;
	}
	return len;
}

int check(int n[], int len)
{
	int i;

	for(i=0; i<len/2; i++)
		if(n[i] != n[len-i-1])
			return 0;
	return 1;
}

void output(int n[], int len, int val[], int l)
{
	int i;

	for(i=l-1; i>=0; i--)
		printf("%c", sign[val[i]]);
	printf(" ");
	for(i=len-1; i>=0; i--)
		printf("%c", sign[n[i]]);
	printf("\n");
}

void square(int val[], int len)
{
	int n[22], l=len+len-1;
	int i, j;

	memset(n, 0, sizeof(n));
	for(i=0; i<len; i++){
		for(j=0; j<len; j++){
			n[i+j] += val[i]*val[j];
		}
	}
	for(i=0; i<l; i++){
		n[i+1] += n[i]/base;
		n[i] %= base;
	}
	while(n[l]){
		n[l+1] += n[i]/base;
		n[l] %= base;
		l++;
	}
	if(check(n, l))
		output(n, l, val, len);
}

int main(void)
{
	int val[10];
	int i, len;
////	freopen("palsquare.in", "r", stdin);
////	freopen("palsquare.out", "w", stdout);

	initial();
	while(scanf("%d", &base) != EOF){
		len = 1;
		memset(val, 0, sizeof(val));
		for(i=0; i<300; i++){
			len = add(val, len);
			square(val, len);
		}
	}
	return 0;

}
//### END
