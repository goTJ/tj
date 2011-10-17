/* @JUDGE_ID:   10319NX 320 C */
#include<stdio.h>

char map[35][35];
int way[4][2] = { 1,0, 0,1, -1,0, 0,-1 };
char sign[256];

int getline(char s[])
{
	int l=0;
	char c;

	while((c=getchar()) != EOF){
		if(c == '\n'){
			s[l] = '\0';
			return l;
		}
		s[l++] = c;
	}
	return -1;
}

void initial(void)
{
	int i, j;

	sign['N'] = 0;
	sign['E'] = 1;
	sign['S'] = 2;
	sign['W'] = 3;
	for(i=0; i<32; i++){
		for(j=0; j<32; j++)
			map[i][j] = '.';
		map[i][32] = '\0';
	}
}

int main(void)
{
	int times, round;
	int x, y;
	char in[1100];
	int i, len, c;
	int j;

	scanf("%d\n", &round);
	for(times=0; times<round; times++){
		if(times)
			printf("\n");
		initial();
		printf("Bitmap #%d\n", times+1);
		scanf("%d %d\n", &y, &x);
		len = getline(in);
		for(i=0; i<len&&in[i]!='.'; i++){
			c = (sign[in[i]]+1)%4;
			if(sign[in[i]] >= 2){
				x = x+way[sign[in[i]]][0];
				y = y+way[sign[in[i]]][1];
			}
			if(c < 2)
				map[x][y] = 'X';
			else
				map[x+way[c][0]][y+way[c][1]] = 'X';
			if(sign[in[i]] < 2){
				x = x+way[sign[in[i]]][0];
				y = y+way[sign[in[i]]][1];
			}
		}
		for(i=31; i>=0; i--)
			printf("%s\n", map[i]);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
