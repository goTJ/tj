/* @JUDGE_ID:   10319NX 10063 C++ */
#include<stdio.h>
#include<string.h>

void reverse(char s[], int len)
{
	int i;
	int tmp;

	for(i=0; i<len/2; i++){
		tmp = s[i];
		s[i] = s[len-i-1];
		s[len-i-1] = tmp;
	}
}

void swap(char *a, char *b)
{
	char tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void insert(char s[], int len, int n)
{
	int i;
	int tmp;

	if(n >= 0){
		insert(s, len, n-1);
		for(i=n+1; i<len; i++){
			swap(&s[i], &s[i-1]);
			insert(s, len, n-1);
		}
		tmp = s[len-1];
		for(i=len-1; i>n; i--)
			s[i] = s[i-1];
		s[n] = tmp;
	}
	else{
		printf("%s\n", s);
	}
}

int main(void)
{
	char s[15];
	int len;
	int times=0;

	while(scanf("%s", s) != EOF){
		if(times)
			printf("\n");
		len = strlen(s);
		reverse(s, len);
		insert(s, len, len-1);
		times = 1;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
