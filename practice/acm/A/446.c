/* @JUDGE_ID:   10319NX 446 C */
#include<stdio.h>
#include<string.h>

int tab[260];

void initial(void)
{
	int i;

	for(i=0; i<26; i++)
		tab[i+'A'] = 10+i;
	for(i=0; i<10; i++)
		tab[i+'0'] = i;
	tab[0] = 0;
}

void reverse(char s[], int len)
{
	int i;
	char tmp;

	for(i=0; i<len/2; i++){
		tmp = s[i];
		s[i] = s[len-i-1];
		s[len-i-1] = tmp;
	}
}

void print_bin(int n, int deep)
{
	if(deep){
		print_bin(n/2, deep-1);
		printf("%d", n%2);
	}
	else
		printf("%d", n%2);
}

void hex2bin(char hex[], int len)
{
	int i, j, n;

	printf("0");
	for(i=len-1; i>=0; i--){
		n = tab[hex[i]];
		print_bin(n, 3);
	}
}

void hex_add(char a[], int la, char b[], int lb, int sign)
{
	int i;
	int sum=0;

	for(i=2; i>=0; i--)
		sum = sum*16+tab[a[i]]+sign*tab[b[i]];
	printf("%d", sum);
}

int main(void)
{
	char a[5], b[5], c;
	int round, times;
	int i, j, sign;
	int len_a, len_b;

	initial();
	scanf("%d", &round);
	for(times=0; times<round; times++){
		memset(a, 0, sizeof(a));
		memset(b, 0, sizeof(b));
		sign = 1;
		scanf("%s %c %s", a, &c, b);
		len_a = strlen(a);
		len_b = strlen(b);
		reverse(a, len_a);
		reverse(b, len_b);
		hex2bin(a, 3);
		printf(" %c ", c);
		hex2bin(b, 3);
		if(c == '-')
			sign = -1;
		printf(" = ");
		hex_add(a, 3, b, 3, sign);
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
