//### PROGRAM
/*
ID: flyherm003
PROG: contact
start: 
finish: 
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct fly{
	int len;
	int val;
	int times;
}tab[13][5000];
int input[200005];
int lim=0;

void init(void)
{
	int i, j;

	for(i=0; i<13; i++)
		for(j=0; j<5000; j++){
			tab[i][j].len = i;
			tab[i][j].val = j;
			tab[i][j].times = 0;
		}
}

void get(int from, int to)
{
	int i, sum=0;

	for(i=from; i<=to; i++){
		sum = sum*2+input[i]-'0';
	}
	tab[to-from][sum].times++;
}

void find(int len)
{
	int i=len-1;

	for(; i<lim; i++)
		get(i-len+1, i);
}

int comp(const void *p, const void *q)
{
	if(((struct fly *)q)->times > ((struct fly *)p)->times)
		return 1;
	else if(((struct fly *)q)->times < ((struct fly *)p)->times)
		return -1;

	if(((struct fly *)q)->len > ((struct fly *)p)->len)
		return -1;
	else if(((struct fly *)q)->len < ((struct fly *)p)->len)
		return 1;

	if(((struct fly *)q)->val > ((struct fly *)p)->val)
		return -1;
	else if(((struct fly *)q)->val < ((struct fly *)p)->val)
		return 1;
}

int itoa(int val, int len)
{
	int i;
	char s[13];

	memset(s, 0, sizeof(s));
	for(i=0; val>0; i++){
		s[i] = val%2;
		val /= 2;
	}
	for(i=len; i>=0; i--)
		printf("%d", s[i]);
}

void output(int num)
{
	int i=0, n=0, tmp, m=0, bug;

	while(n<num && (tab[m][i].times)!=0){
		bug = 0;
		printf("%d\n", tab[m][i].times);
		tmp = i;
		itoa(tab[m][i++].val, tab[m][i].len);
		bug += tab[m][i].len;
		while(tab[m][i].times == tab[m][tmp].times){
			bug += tab[m][i].len+1;
			if(bug > 80){
				printf("\n");
				bug = tab[m][i].len+1;
			}
			else
				printf(" ");
			itoa(tab[m][i].val, tab[m][i].len);
			if(++i == 5000){
				i = 0;
				m++;
			}
		}
		n++;
		printf("\n");
	}
}

int main(void)
{
	int A, B, N, i;
	char c;
	freopen("contact.in",  "r",  stdin);
	freopen("contact.out",  "w",  stdout);

	init();
	scanf("%d%d%d", &A, &B, &N);
	while(scanf("%c", &c) != EOF){
		if(c != '\n')
			input[lim++] = c;
	}
	for(i=A; i<=B; i++)
		find(i);
	qsort(tab, 13*5000, sizeof(tab[0][0]), comp);
	output(N);
	return 0;
}
//### END
