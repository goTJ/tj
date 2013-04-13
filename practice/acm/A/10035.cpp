#include<stdio.h>
#include<string.h>

inline void reverse(char s[], const int &len)
{
	char tmp;

	for(int i=0; i<len/2; i++){
		tmp = s[i];
		s[i] = s[len-i-1];
		s[len-i-1] = tmp;
	}
}

inline void atoi_fly(char s[], const int &len)
{
	for(int i=0; i<len; i++)
		s[i] -= '0';
}

inline int max(const int &a, const int &b)
{
	return (a>b) ? a : b;
}

int main()
{
	char a[20]={0}, b[20];
	int la, lb, maxl;
	int count;

	while(scanf("%s%s", a, b) == 2){
		la = strlen(a);
		lb = strlen(b);
		if(la==1 && a[0]=='0' && lb==1 && b[0]=='0')
			break;
		reverse(a, la);
		reverse(b, lb);
		atoi_fly(a, la);
		atoi_fly(b, lb);
		for(int i=0; i<lb; i++)
			a[i] += b[i];
		maxl = max(la, lb);
		count = 0;
		for(int i=0; i<maxl; i++){
			count += a[i]/10;
			a[i+1] += a[i]/10;
			a[i] %= 10;
		}
		if(count == 0)
			printf("No carry operation.\n");
		else if(count == 1)
			printf("1 carry operation.\n");
		else
			printf("%d carry operations.\n", count);
		memset(a, 0, sizeof(a));
	}
	return 0;
}
