// vim:ft=c:foldmethod=marker
#include<stdio.h>

int main()
{
	int c;
	int i=0;

	while((c=getchar()) != EOF){
		printf((c&128) ? "1" : "0");
		printf((c&64) ? "1" : "0");
		printf((c&32) ? "1" : "0");
		printf((c&16) ? "1" : "0");
		printf((c&8) ? "1" : "0");
		printf((c&4) ? "1" : "0");
		printf((c&2) ? "1" : "0");
		printf((c&1) ? "1" : "0");
		i++;
		if(!(i%4))
			printf("\n");
	}
	if((i%4))
		printf("\n");
	return 0;
}
