/*   @JUDGE_ID:   10319NX   656   C++*/
#include<stdio.h>
#include<stdlib.h>
#define MAX 10

int begin[MAX], end[MAX];
int bestcmd[MAX], bestlen;
int lim;

void go(int depth, int len, int *s[], int cmd[])
{
	int next[MAX];
	int i, j;
	int *tmp;

	// check
	if(depth >= bestlen)
		return;
	for(i=0; i<lim; i++)
		if(s[len][i] != end[i])
			break;
	if(i == lim){
		bestlen = depth;
		for(i=0; i<bestlen; i++)
			bestcmd[i] = cmd[i];
	}
	if(i == 10)
		return;
	// find next
	for(j=0; j<5; j++){
		if(j==2 && len+depth>=10)
			continue;
		if(j!=2 && len==0)
			continue;
		cmd[depth] = j;
		switch(j){
			case 0: // ADD
				for(i=0; i<lim; i++)
					next[i] = s[len-1][i] + s[len][i];
				break;
			case 1:	// DIV
				for(i=0; i<lim; i++){
					if(s[len][i])
						next[i] = s[len-1][i] / s[len][i];
					else
						goto skip;
				}
				break;
			case 2: // DUP
				tmp = s[len+1];
				s[len+1] = s[len];
				go(depth+1, len+1, s, cmd);
				s[len+1] = tmp;
				continue;
			case 3: // MUL
				for(i=0; i<lim; i++)
					next[i] = s[len-1][i] * s[len][i];
				break;
			case 4: // SUB
				for(i=0; i<lim; i++)
					next[i] = s[len-1][i] - s[len][i];
				break;
		}
		for(i=0; i<lim; i++){	// check
			if(abs(next[i]) > 30000)
				goto skip;
		}
		tmp = s[len-1];
		s[len-1] = next;
		go(depth+1, len-1, s, cmd);
		s[len-1] = tmp;
skip:
		;
	}
}

void output(void)
{
	int i;

	for(i=0; i<bestlen; i++){
		switch(bestcmd[i]){
			case 0:
				printf("ADD");
				break;
			case 1:
				printf("DIV");
				break;
			case 2:
				printf("DUP");
				break;
			case 3:
				printf("MUL");
				break;
			case 4:
				printf("SUB");
				break;
		}
		printf(" ");
	}
	printf("\n");
}

int main(void)
{
	int round=1;
	int *s[MAX];
	int cmd[MAX];
	int i;

	while(scanf("%d", &lim) != EOF){
		if(!lim)
			break;
		// input
		for(i=0; i<lim; i++)
			scanf("%d", &begin[i]);
		for(i=0; i<lim; i++)
			scanf("%d", &end[i]);
		// initial
		s[0] = begin;
		bestlen = 11;
		// process
		go(0, 0, s, cmd);
		// output
		printf("Program %d\n", round);
		if(bestlen == 11)
			printf("Impossible\n");
		else if(bestlen == 0)
			printf("Empty sequence\n");
		else
			output();
		printf("\n");
		round++;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
