#include<cstdio>
#include <cstring>

inline void makeTable(char table[])
{
	char *tmp1="1234567890-=WERTYUIOP[]\\SDFGHJKL;'XCVBNM,./";
	char *tmp2="`1234567890-QWERTYUIOP[]ASDFGHJKL;ZXCVBNM,.";

	for(int i=0; i<300; i++)
		table[i] = i;
	int len=strlen(tmp1);
	for(int i=0; i<len; i++)
		table[tmp1[i]] = tmp2[i];
}

int main()
{
	char transfer[300];
	makeTable(transfer);
	char c;
	while((c=getchar()) != EOF)
		putchar(transfer[(int)c]);
	return 0;
}
