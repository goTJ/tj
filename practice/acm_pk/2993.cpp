#include<cstdio>
#include<cstring>
#include <cctype>

int main()
{
	char buf[222];
	char map[8][8]={0};

	for(int i=0; i<2; i++){
		scanf("%s", buf);
		scanf("%s", buf);
		char *p=strtok(buf, ",");
		do{
			char c;
			if(strlen(p) == 3){
				c = p[0];
				p++;
			}else{
				c = 'P';
			}
			if(i)
				c = tolower(c);
			map[8-p[1]+'0'][p[0]-'a'] = c;
		}while((p=strtok(NULL, ",")));
	}
	for(int i=0; i<8; i++){
		printf("+---+---+---+---+---+---+---+---+\n");
		printf("|");
		for(int j=0; j<8; j++){
			char c=((i+j)%2)?':':'.';
			printf("%c", c);
			printf("%c", (map[i][j])?map[i][j]:c);
			printf("%c|", c);
		}
		printf("\n");
	}
	printf("+---+---+---+---+---+---+---+---+");
	return 0;
}
