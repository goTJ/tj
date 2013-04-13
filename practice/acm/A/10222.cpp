/* @JUDGE_ID:   10319NX 10222 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>
#include<string.h>

char sign[256];

void initial(void)
{
	memset(sign, 0, sizeof(sign));
	sign['e'] = 'q';
	sign['d'] = 'a';
	sign['c'] = 'z';
	sign['r'] = 'w';
	sign['f'] = 's';
	sign['v'] = 'x';
	sign['t'] = 'e';
	sign['g'] = 'd';
	sign['b'] = 'c';
	sign['y'] = 'r';
	sign['h'] = 'f';
	sign['n'] = 'v';
	sign['u'] = 't';
	sign['j'] = 'g';
	sign['m'] = 'b';
	sign['i'] = 'y';
	sign['k'] = 'h';
	sign[','] = 'n';
	sign['o'] = 'u';
	sign['l'] = 'j';
	sign['.'] = 'm';
	sign['p'] = 'i';
	sign[';'] = 'k';
	sign['/'] = ',';
	sign['['] = 'o';
	sign['\''] = 'l';
	sign[']'] = 'p';
	sign['E'] = 'q';
	sign['D'] = 'a';
	sign['C'] = 'z';
	sign['R'] = 'w';
	sign['F'] = 's';
	sign['V'] = 'x';
	sign['T'] = 'e';
	sign['G'] = 'd';
	sign['B'] = 'c';
	sign['Y'] = 'r';
	sign['H'] = 'f';
	sign['N'] = 'v';
	sign['U'] = 't';
	sign['J'] = 'g';
	sign['M'] = 'b';
	sign['I'] = 'y';
	sign['K'] = 'h';
	sign['<'] = 'n';
	sign['O'] = 'u';
	sign['L'] = 'j';
	sign['>'] = 'm';
	sign['P'] = 'i';
	sign[':'] = 'k';
	sign['?'] = ',';
	sign['{'] = 'o';
	sign['"'] = 'l';
	sign['}'] = 'p';
}

int main(void)
{
	char c;

	initial();
	while((c=getchar()) != EOF){
		if(sign[c])
			printf("%c", sign[c]);
		else
			printf("%c", c);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
