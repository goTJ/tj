// vim:ft=cpp:foldmethod=marker
#include <cstdio>

void gogogo(int y, int x, char *s, int depth)
{
	if(depth == 8){
		printf("%s ", s);
	}else{
		if(depth&1){
			if(x & (1<<(3-depth/2)))
				s[depth] = '1';
			else
				s[depth] = '0';
		}else{
			if(y & (1<<(3-depth/2)))
				s[depth] = '0';
			else
				s[depth] = '1';
		}
		gogogo(y, x, s, depth+1);
	}
}

int main()
{
	char buf[10];
	buf[8] = '\0';
	printf("8\n");
	for(int i=0; i<16; i++){
		for(int j=0; j<16; j++)
			gogogo(i, j, buf, 0);
		printf("\n");
	}
	return 0;
}
