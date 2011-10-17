#include<cstdio>
#include<cstring>
#include<cctype>

int main()
{
	char buf[222];
	char row='8';
	char white[6][6][6];
	int wl[6];
	char black[6][6][6];
	int bl[6];
	int tab[256];
	char *tab2[] = { "K", "Q", "R", "B", "N", "" };

	tab['k'] = tab['K'] = 0;
	tab['q'] = tab['Q'] = 1;
	tab['r'] = tab['R'] = 2;
	tab['b'] = tab['B'] = 3;
	tab['n'] = tab['N'] = 4;
	tab['p'] = tab['P'] = 5;
	for(int i=0; i<6; i++)
		wl[i] = bl[i] = 0;
	while(scanf("%s", buf) == 1){
		if(!strcmp(buf, "+---+---+---+---+---+---+---+---+"))
			continue;
		char *p=strtok(buf, "|");
		char col='a';
		char stack[8][4];
		int sl=0;
		do{
			if(islower(p[1])){
				int idx=tab[p[1]];
				black[idx][bl[idx]][0] = col;
				black[idx][bl[idx]][1] = row;
				black[idx][bl[idx]][2] = '\0';
				bl[idx]++;
			}else if(isupper(p[1])){
				stack[sl][0] = p[1];
				stack[sl][1] = col;
				stack[sl][2] = row;
				stack[sl][3] = '\0';
				sl++;
			}
			col++;
		}while((p=strtok(NULL, "|")));
		for(int i=sl-1; i>=0; i--){
			int idx=tab[stack[i][0]];
			white[idx][wl[idx]][0] = stack[i][1];
			white[idx][wl[idx]][1] = stack[i][2];
			white[idx][wl[idx]][2] = '\0';
			wl[idx]++;
		}
		row--;
	}
	printf("White: ");
	bool first=true;
	for(int i=0; i<6; i++){
		for(int j=wl[i]-1; j>=0; j--){
			if(!first)
				printf(",");
			else
				first = false;
			white[i][j][2] = '\0';
			printf("%s%s", tab2[i], white[i][j]);
		}
	}
	printf("\n");
	printf("Black: ");
	first=true;
	for(int i=0; i<6; i++){
		for(int j=0; j<bl[i]; j++){
			if(!first)
				printf(",");
			else
				first = false;
			black[i][j][2] = '\0';
			printf("%s%s", tab2[i], black[i][j]);
		}
	}
	return 0;
}
