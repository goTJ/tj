/*###########################################################################*/
/*       title: E                                                            */
/*                                                                           */
/*           start: */
/*                                                                           */
/*                finish: */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   325   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>
#include<string.h>
#include<ctype.h>

enum { NEXT = 257 };
int tab[258];
void (*change[258])(void);

void num_1(void)
{
	int i;

	tab['+'] = 0;
	tab['-'] = 0;
	tab['.'] = 1;
	tab['e'] = 1;
	tab['E'] = 1;
	tab[NEXT] = 1;
	for(i='0'; i<='9'; i++)
		tab[i] = 1;
}

void num_2(void)
{
	int i;

	tab['+'] = 0;
	tab['-'] = 0;
	tab['.'] = 0;
	tab['e'] = 1;
	tab['E'] = 1;
	tab[NEXT] = 0;
	for(i='0'; i<='9'; i++)
		tab[i] = 1;
}

void num_3(void)
{
	int i;

	tab['+'] = 0;
	tab['-'] = 0;
	tab['.'] = 0;
	tab['e'] = 0;
	tab['E'] = 0;
	tab[NEXT] = 0;
	for(i='0'; i<='9'; i++)
		tab[i] = 1;
}

void add(void)
{
	int i;

	tab['+'] = 0;
	tab['-'] = 0;
	tab['.'] = 0;
	tab['e'] = 0;
	tab['E'] = 0;
	tab[NEXT] = 1;
	for(i='0'; i<='9'; i++)
		tab[i] = 1;
}

void sub(void)
{
	int i;

	tab['+'] = 0;
	tab['-'] = 0;
	tab['.'] = 0;
	tab['e'] = 0;
	tab['E'] = 0;
	tab[NEXT] = 1;
	for(i='0'; i<='9'; i++)
		tab[i] = 1;
}

void dot(void)
{
	int i;

	tab['+'] = 0;
	tab['-'] = 0;
	tab['.'] = 0;
	tab['e'] = 0;
	tab['E'] = 0;
	tab[NEXT] = 1;
	for(i='0'; i<='9'; i++){
		tab[i] = 1;
		change[i] = num_2;
	}
}

void e(void)
{
	int i;

	tab['+'] = 1;
	tab['-'] = 1;
	tab['.'] = 0;
	tab['e'] = 0;
	tab['E'] = 0;
	tab[NEXT] = 1;
	for(i='0'; i<='9'; i++){
		tab[i] = 1;
		change[i] = num_3;
	}
}

void init(void)
{
	int i;

	memset(tab, 0, sizeof(tab));
	tab['+'] = 1;
	tab['-'] = 1;
	tab[NEXT] = 1;
	for(i='0'; i<='9'; i++){
		tab[i] = 1;
		change[i] = num_1;
	}
	change['+'] = add;
	change['-'] = sub;
	change['.'] = dot;
	change['e'] = e;
	change['E'] = e;
}

int tell(char s[])
{
	int i;

	for(i=0; isspace(s[i]); i++);
	while(s[i] != '\0'){
		if(!tab[(int)s[i]])
			return 0;
		change[(int)s[i]]();
		i++;
	}
	if(tab[NEXT])
		return 0;
	return 1;
}

int main(void)
{
	char s[83];

	while(scanf("%s", s) != EOF){
		init();
		if(s[0] == '*')
			break;
		printf("%s is ", s);
		printf(tell(s) ? "legal.\n" : "illegal.\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
