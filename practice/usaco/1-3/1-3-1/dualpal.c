//### PROGRAM
/*
ID: flyherm003
PROG: dualpal
*/
#include<stdio.h>

int transcript(int num, int base, int s[])
{
	int i;

	for(i=0; num; i++){
		s[i] = num%base;
		num /= base;
	}
	return i;
}

int isPal(int s[], int len)
{
	int i;

	for(i=0; i<len/2; i++){
		if(s[i] != s[len-i-1])
			return 0;
	}
	return 1;
}

int check(int n)
{
	int buf[20];
	int len;
	int i, flag=0;

	for(i=2; i<=10; i++){
		len = transcript(n, i, buf);
		if(isPal(buf, len))
			flag++;
	}
	if(flag >= 2)
		return 1;
	else
		return 0;
}

int main(void)
{
	int first, times;
	int i;
/**/	freopen("dualpal.in", "r", stdin);
/**/	freopen("dualpal.out", "w", stdout);

	while(scanf("%d%d", &times, &first) != EOF){
		for(i=first+1; times; i++){
			if(check(i)){
				printf("%d\n", i);
				times--;
			}
		}
	}
	return 0;
}
//### END
