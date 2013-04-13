#include<cstdio>
#include<cctype>
#include<cstdlib>

int main()
{
	char tmp;
	int n=0;

	while((tmp=getchar()) != EOF){
		if(tmp == '\n' || tmp == '!'){
			putchar('\n');
		}else if(isdigit(tmp)){
			n += tmp-'0';
		}else if(isalpha(tmp) || tmp=='*'){
			char pattern=(tmp=='b') ? ' ' : tmp;
			tmp = '\0';
			for(; n>0; n--)
				putchar(pattern);
		}
	}
	return 0;
}
