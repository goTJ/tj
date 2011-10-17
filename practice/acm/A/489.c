#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int check(int tab[])
{
	int i;

	for(i=0; i<26; i++)
		if(tab[i] == 1)
			return 0;
	return 1;
}

int main(void)
{
	int tab[27];
	int times, n, round, flag;
	char c;

	round = 1;
	while(scanf("%d\n", &round) != EOF){
		times = 0;
		if(round == -1)
			exit(0);
		printf("Round %d\n", round);
		flag = 1;
		memset(tab, 0, sizeof(tab));
		while((c=getchar()) != '\n')
			tab[c-'a'] = 1;
		while((c=getchar()) != '\n'){
			n = c-'a';
			if(tab[n])
				tab[n] = 2;
			else
				times++;
			if(times == 7){
				printf("You lose.\n");
				flag = 0;
				while((c=getchar()) != '\n');
				break;
			}
			if(check(tab)){
				printf("You win.\n");
				flag = 0;
				while((c=getchar()) != '\n');
				break;
			}
		}
		if(flag)
			printf("You chickened out.\n");
	}
	return 0;
}
