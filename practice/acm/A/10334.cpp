/* @JUDGE_ID: 10319NX 10334 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#define RANGE 4
#define MAX_LENG 1000

typedef struct BigNum{
	int num[MAX_LENG];
	int len;
	int sign;
}BigNum;
BigNum fab[1001];

int strNum(BigNum *a, char buf[])
{
	int i, len;
	int flag=0;

	a->sign = 1;
	if(buf[0] == '-'){
		a->sign = -1;
		flag = 1;
	}
	len = strlen(buf)-flag;
	a->len = (len-1)/RANGE+1;
	if(a->len > MAX_LENG)
		return EOF;
	for(i=0; i<a->len; i++)
		a->num[i] = 0;
	for(i=0; i<len; i++){
		if(!isdigit(buf[i+flag]))
			return EOF;
		a->num[(len-i-1)/RANGE] = a->num[(len-i-1)/RANGE]*10 + buf[i+flag]-'0';
	}
	return a->len;
}

int numNum(BigNum *a, int n)
{
	char buf[1000];

	sprintf(buf, "%d", n);
	return strNum(a, buf);
}

int printNum(BigNum a)
{
	int i;

	if(a.sign < 0)
		printf("-");
	if(a.len < 1)
		return -1;
	printf("%d", a.num[a.len-1]);
	for(i=a.len-2; i>=0; i--)
		printf("%0*d", RANGE, a.num[i]);
	return a.len;
}

int cmpNumX(BigNum a, BigNum b)
{
	int i;

	if(a.len > b.len)
		return 1;
	else if(a.len > b.len)
		return -1;
	else{
		for(i=a.len-1; i>=0; i--){
			if(a.num[i] != b.num[i])
				return a.num[i]-b.num[i];
		}
		return 0;
	}
}

int cmpNum(BigNum a, BigNum b)
{
	if(a.sign>0 && b.sign<0)
		return 1;
	else if(a.sign<0 && b.sign>0)
		return -1;
	else if(a.sign>0 && b.sign>0)
		return cmpNumX(a, b);
	else
		return cmpNumX(b, a);
}

BigNum sortNum(BigNum a)
{
	int i;
	int base;

	for(i=0,base=1; i<RANGE; i++)
		base *= 10;
	for(i=0; i<a.len; i++){
		a.num[i+1] += a.num[i]/base;
		a.num[i] = a.num[i]%base;
		if(a.num[i] < 0){
			a.num[i+1] -= 1;
			a.num[i] += base;
		}
	}
	for(i=a.len; i>0; i--)
		if(a.num[i])
			break;
	a.len = i+1;
	return a;
}

BigNum addNumX(BigNum a, BigNum b, int sign)
{
	int len, i;

	len = (a.len > b.len) ? a.len : b.len;
	for(i=a.len; i<b.len; i++)
		a.num[i] = 0;
	a.num[len] = 0;
	for(i=0; i<b.len; i++)
		a.num[i] += b.num[i]*sign;
	a.len = len;
	a = sortNum(a);
	return a;
}

BigNum addNum(BigNum a, BigNum b)
{
	int sign=a.sign*b.sign;
	int t;

	if(sign > 0)
		return addNumX(a, b, 1);
	else{
		if((t=cmpNumX(a, b)) > 0)
			return addNumX(a, b, -1);
		else if(t < 0)
			return addNumX(b, a, -1);
		else{
			a.num[0] = 0;
			a.sign = 1;
			a.len = 1;
			return a;
		}
	}
}

void initial(void)
{
	int i;

	numNum(&fab[0], 1);
	numNum(&fab[1], 2);
	for(i=2; i<=1000; i++)
		fab[i] = addNum(fab[i-1], fab[i-2]);
}

int main(void)
{
	int n;

	initial();
	while(scanf("%d", &n) != EOF){
		printNum(fab[n]);
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
