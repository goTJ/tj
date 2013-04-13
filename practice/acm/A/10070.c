/* @JUDGE_ID:   10319NX 10070 C */
#include<stdio.h>
#include<stdlib.h>

int getline(char in[])
{
	char c;
	int l=0;

	while((c=getchar()) != EOF){
		if(c == '\n'){
			in[l] = '\0';
			return l;
		}
		in[l++] = c;
	}
	return -1;
}

int mod100(char s[], int len)
{
	int val=(s[len-2]-'0')*10+(s[len-1]-'0');

	return val;
}

int mod4(char s[], int len)
{
	int val=(s[len-2]-'0')*10+(s[len-1]-'0');

	return val%4;
}

int mod400(char s[], int len)
{
	if(!mod100(s, len) && !mod4(s, len-2))
		return 0;
	else
		return 1;
}

int mod15(char s[], int len)
{
	int sum=0;
	int i;

	if(s[len-1]!='0' && s[len-1]!='5')
		return 1;
	for(i=0; i<len; i++)
		sum += s[i]-'0';
	return sum%3;
}

int mod55(char s[], int len)
{
	int a=0, b=0, i;

	if(s[len-1]!='0' && s[len-1]!='5')
		return 1;
	for(i=0; i<len; i++){
		if(i%2)
			a += s[i]-'0';
		else
			b += s[i]-'0';
	}
	a = abs(a-b);
	return a%11;
}

int main(void)
{
	char in[10000];
	int len;
	int leap, nothing, times=0;

	while((len=getline(in)) != -1){
		if(!len)
			continue;
		leap = 0;
		nothing = 1;
		if(times)
			printf("\n");
		if(!mod4(in, len) && ( mod100(in, len) || !mod400(in, len) )){
			printf("This is leap year.\n");
			leap = 1;
			nothing = 0;
		}
		if(!mod15(in, len)){
			printf("This is huluculu festival year.\n");
			nothing = 0;
		}
		if(!mod55(in, len) && leap){
			printf("This is bulukulu festival year.\n");
			nothing = 0;
		}
		if(nothing)
			printf("This is an ordinary year.\n");
		times++;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
