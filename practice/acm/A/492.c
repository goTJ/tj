/*   @JUDGE_ID:   10319NX   492   C */
#include<stdio.h>

int ismom(char a)
{
	if(a=='a'||a=='e'||a=='i'||a=='o'||a=='u')
		return 1;
	if(a=='A'||a=='E'||a=='I'||a=='O'||a=='U')
		return 1;
	return 0;
}

int iseng(char c)
{
	if(c>='a'&&c<='z' || c>='A'&&c<='Z')
		return 1;
	return 0;
}

int main(void)
{
	char c;
	char *p = "ay";
	char t[2];
	int eng=0;

	t[0] = t[1] = '\0';
	while(scanf("%c", &c) != EOF){
		if(iseng(c) && eng){
			printf("%c", c);
		}
		if(!iseng(c) && !eng){
			printf("%c", c);
		}
		if(iseng(c) && !eng){
			eng = 1;
			if(!ismom(c))
				t[0] = c;
			else
				printf("%c", c);
		}
		if(!iseng(c) && eng){
			eng = 0;
			printf("%s%s%c", t, p, c);
			t[0] = '\0';
		}
	}
	if(eng)
		printf("%s%s", t, p);
	return 0;
}
/* @END_OF_SOURCE_CODE */
