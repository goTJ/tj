/* @JUDGE_ID:   10319NX 10098 C++ "¦ó¦Ì¯S^^" */
#include<iostream>
#include<string.h>
#include<stdlib.h>

int sign[256];

void initial(void)
{
	int i;

	for(i=0; i<26; i++){
		sign['A'+i] = i;
		sign['a'+i] = i;
	}
}

void swap(char *a, char *b)
{
	char t;

	t = *a;
	*a = *b;
	*b = t;
}

int comp(const void *p, const void *q)
{
	return *(char *)p - *(char *)q;
}

int find(char s[], int len)
{
	int i;

	for(i=len-2; i>=0; i--)
		if(sign[s[i]] < sign[s[i+1]])
			return i;
	return -1;
}

int next(char s[], int from, int to, char c)
{
	int i;
	int max=30, pos;

	for(i=from; i<to; i++){
		if(sign[s[i]]>sign[c] && sign[s[i]]<max){
			max = sign[s[i]];
			pos = i;
		}
	}
	return pos;
}

int main(void)
{
	int round, times;
	char s[13];
	int len;
	int pos, n;

	initial();
	cin >> round;
	for(times=0; times<round; times++){
		cin >> s;
		len = strlen(s);
		qsort(s, len, sizeof(s[0]), comp);
		cout << s << endl;
		while((pos=find(s, len)) != -1){
			n = next(s, pos+1, len, s[pos]);
			swap(&s[pos], &s[n]);
			qsort(s+pos+1, len-pos-1, sizeof(s[0]), comp);
			cout << s << endl;
		}
		cout << endl;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
