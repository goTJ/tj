/* @JUDGE_ID:   10319NX 424 C */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

int add(char a[], int la, char b[], int lb)
{
	int i, max;

	for(i=0; i<lb; i++)
		a[i] += b[i]-'0';
	max = (la>lb) ? la : lb;
	for(i=0; i<max; i++)
		if(a[i] >= 10){
			a[i+1] += a[i]/10;
			a[i] %= 10;
		}
	if(a[i] > 0)
		return max+1;
	return max;
}

int main(void)
{
	char a[500];
	char tmp[102];
	int i, len_a, len_t;

	memset(a, 0, sizeof(a));
	len_a = 0;
	while(scanf("%s", tmp) != EOF){
		len_t = strlen(tmp);
		reverse(tmp, len_t);
		if(len_t==1 && tmp[0]=='0')
			break;
		len_a = add(a, len_a, tmp, len_t);
	}
	for(i=len_a-1; i>=0; i--)
		printf("%d", a[i]);
	printf("\n");
	return 0;
}
/* @END_OF_SOURCE_CODE */
