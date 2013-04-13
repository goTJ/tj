//### PROGRAM
/*
ID: flyherm003
PROG: beads
*/
#include<stdio.h>

char s[350];
int max;

int circle(int n)
{
	if(n>=max)
		return n-max;
	else if(n<0)
		return max+n;
	else 
		return n;
}

void input(void)
{
	FILE *in = fopen("beads.in", "r");

	fscanf(in, "%d%s", &max, s);
}

int tell(int n, char fly)
{
	if(s[n] == fly || s[n] == 'w')
		return 1;
	return 0;
}

int find(int n, int a)
{
	while(s[n]=='w')
		n += a;
	return n;
}

int judge(int doing)
{
	int i, j, jj, sum = 0;
	char fly;

	jj=doing;
	fly=s[find(jj, -1)];
	while(tell(jj, fly)){
		sum++;
		jj=circle(jj-1);
		if(jj==circle(doing)){
			break;
		}
	}
	jj=circle(jj+1);
	for(j=circle(doing+1), fly=s[find(j, 1)];
			tell(j, fly)&&j!=circle(jj); j=circle(j+1)){
			sum++;
	}
	return sum;
}
int main(void)
{
	int i, sum = 0, t;	
	FILE *out = fopen("beads.out", "w");

	input();
	for(i=0; i<max; i++)
		if(sum < (t = judge(i)))
			sum = t;
	fprintf(out, "%d\n", sum);
	return 0;
}
//###END
