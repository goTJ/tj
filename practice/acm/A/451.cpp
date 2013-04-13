/*   @JUDGE_ID:   10319NX   451 "§Ú°Q¹½ÂÅ¤Ä-.-"  C++*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum { N, OP, TP, TK, S, F, FH, FK, SF };

typedef struct card{
	int kind;
	int value;
}Card;

Card tab[5][5];
int sign[256];
int num[9];

void initSign(void)
{
	int i;

	for(i=1; i<=9; i++)
		sign[i+'0'] = i;
	sign['A'] = 1;
	sign['X'] = 10;
	sign['J'] = 11;
	sign['Q'] = 12;
	sign['K'] = 13;

	sign['S'] = 3;
	sign['H'] = 2;
	sign['D'] = 1;
	sign['C'] = 0;
}

int comp(const void *p, const void *q)
{
	if(((Card *)p)->value > ((Card *)q)->value)
		return 1;
	if(((Card *)p)->value < ((Card *)q)->value)
		return -1;
	if(((Card *)p)->kind > ((Card *)q)->kind)
		return 1;
	if(((Card *)p)->kind < ((Card *)q)->kind)
		return -1;
	return 0;
}

int judge(Card card[])
{
	int i, pair;
	int k;

	qsort(card, 5, sizeof(card[0]), comp);
	for(i=0; i+3<5; i++)
		if(card[i+3].value == card[i].value)
			return FK;
	if(card[4].value==card[2].value)
		return (card[1].value==card[0].value) ? FH : TK;
	if(card[3].value==card[1].value)
		return TK;
	if(card[2].value==card[0].value)
		return (card[4].value==card[3].value) ? FH : TK;
	for(i=0,pair=0; i<4; i++)
		if(card[i].value==card[i+1].value)
			pair++;
	if(pair)
		return (pair==2) ? TP : OP;
	for(i=0; i<5; i++)
		if(card[i].kind != card[0].kind)
			break;
	if(card[4].value-card[0].value==4)
		return (i==5) ? SF : S;
	for(k=0; k<4; k++)
		if(card[k].value+13-card[k+1].value == 4)
			return (i==5) ? SF : S;
	if(i==5)
		return F;
	return N;
}

int main(void)
{
	int times, round;
	int i, j;
	char buf[5];
	Card card[5];

	initSign();
	scanf("%d", &times);
	for(round=0; round<times; round++){
		memset(num, 0, sizeof(num));
		if(round)
			printf("\n");
		for(i=0; i<5; i++)
			for(j=0; j<5; j++){
				scanf("%s", buf);
				tab[i][j].kind = sign[buf[1]];
				tab[i][j].value = sign[buf[0]];
			}
		for(i=0; i<5; i++){
			for(j=0; j<5; j++)
				card[j] = tab[i][j];
			num[judge(card)]++;
			for(j=0; j<5; j++)
				card[j] = tab[j][i];
			num[judge(card)]++;
		}
		for(i=0; i<8; i++)
			printf("%d, ", num[i]);
		printf("%d\n", num[i]);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */