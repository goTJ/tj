/*   @JUDGE_ID:   10319NX   439   C */
#include<stdio.h>
#include<string.h>

char way[][2] = { 1,2, 2,1, -1,2, 2,-1, -2,1, 1,-2, -2,-1, -1,-2 };
char fc, fr, ec, er, lim;
int map[10][10];
struct jjj{
	char row;
	char col;
	int father;
}tab[10000];

int check(char a, char b)
{
	if(map[a-'a'][b-'1'] == 1)
		return 0;
	if(a>'h' || a<'a')
		return 0;
	if(b>'8' || b<'1')
		return 0;
	return 1;
}

int got(char r, char c)
{
	if(r==er && c==ec)
		return 1;
	return 0;
}

void put(char r, char c, int n)
{
	tab[lim].row = r;
	tab[lim].col = c;
	tab[lim].father = n;
	lim++;
}

int find(int n)
{
	char tr, tc, i;

	for(i=0; i<8; i++){
		tr = tab[n].row+way[i][0];
		tc = tab[n].col+way[i][1];
		if(check(tr, tc)){
			map[tr-'a'][tc-'1'] = 1;
			if(got(tr, tc))
				return 0;
			put(tr, tc, n);
		}
	}
	return 1;
}

int count(int n)
{
	int times=0;

	while(tab[n].father != -1){
		n = tab[n].father;
		times++;
	}
	return times+1;
}

int main(void)
{
	int i;

	while(scanf("\n%c%c %c%c", &fr, &fc, &er, &ec) != EOF){
		if(fc==ec && fr==er)
		   printf("To get from %c%c to %c%c takes 0 knight moves.\n",
					fr, fc, er, ec);
		else{
			memset(map, 0, sizeof(map));
			i = -1;
			lim = 0;
			put(fr, fc, i++);
			while(find(i++));
		   printf("To get from %c%c to %c%c takes %d knight moves.\n",
			fr, fc, er, ec, count(i-1));
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
