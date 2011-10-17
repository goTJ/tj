/*   @JUDGE_ID:   10319NX   131   C++*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum { N, OP, TP, TK, S, F, FH, FK, SF };
char *name[9] = { "highest-card", "one-pair", "two-pairs", "three-of-a-kind", 
"straight", "flush", "full-house", "four-of-a-kind", "straight-flush" };

typedef struct card{
	int kind;
	int value;
}Card;

Card hand[5], deck[5];
int sign[256];
int ans;

void initSign(void)
{
	int i;

	for(i=1; i<=9; i++)
		sign[i+'0'] = i;
	sign['A'] = 1;
	sign['T'] = 10;
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

void insertCard(int kind, int value, Card *s)
{
	s->kind = kind;
	s->value = value;
}

int judge(Card card[])
{
	int i, pair;

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
	if(card[4].value-card[0].value==4 || card[0].value+13-card[1].value == 4)
		return (i==5) ? SF : S;
	if(i==5)
		return F;
	return N;
}

void choose(int d, Card card[], int n)
{
	if(n == 5){
		Card buf[5];
		int tmp;
		memmove(buf, card, sizeof(card[0])*5);
		if((tmp=judge(buf)) > ans)
			ans = tmp;
	}
	else{
		card[n] = hand[n];
		choose(d, card, n+1);
		card[n] = deck[d];
		choose(d+1, card, n+1);
	}
}

int main(void)
{
	char buf[5];
	int i;
	Card card[5];

	initSign();
	while(scanf("%s", buf) != EOF){
		ans = 0;
		printf("Hand: %s ", buf);
		insertCard(sign[buf[1]], sign[buf[0]], &hand[0]);
		for(i=1; i<5; i++){
			scanf("%s", buf);
			printf("%s ", buf);
			insertCard(sign[buf[1]], sign[buf[0]], &hand[i]);
		}
		printf("Deck: ");
		for(i=0; i<5; i++){
			scanf("%s", buf);
			printf("%s ", buf);
			insertCard(sign[buf[1]], sign[buf[0]], &deck[i]);
		}
		choose(0, card, 0);
		printf("Best hand: %s\n", name[ans]);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */