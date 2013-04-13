#include<stdio.h>
#include<string.h>

struct table{
	int block[27];
	int num;
}tab[27];
int lim;
int pos[27];

void initial(void)
{
	int i;

	for(i=0; i<lim; i++){
		pos[i] = i;
		tab[i].block[0] = i;
		tab[i].num = 1;
	}
}

void init(int poss, int n)
{
	int i;
	int t;

	for(i=tab[poss].num-1; tab[poss].block[i]!=n; i--){
		t = tab[poss].block[i];
		pos[t] = t;
		tab[t].block[tab[t].num++] = t;
	}
	tab[poss].num = i+1;
}

void move_top(int posf, int from, int post)
{
	int i;
	int t;

	for(i=0; tab[posf].block[i]!=from; i++)
		;
	t = i;
	for(; i<tab[posf].num; i++){
		tab[post].block[tab[post].num++] = tab[posf].block[i];
		pos[tab[posf].block[i]] = post;
	}
	tab[posf].num = t;
}

void output(void)
{
	int i, j;

	for(i=0; i<lim; i++){
		printf("%d:", i);
		for(j=0; j<tab[i].num; j++)
			printf(" %d", tab[i].block[j]);
		printf("\n");
	}
}

int main(void)
{
	char cmdf[10], cmdt[10];
	int from, to;

	while(scanf("%d", &lim) != EOF){
		initial();
		while(scanf("%s", cmdf) != EOF){
			if(!strcmp(cmdf, "quit")){
				output();
				break;
			}
			scanf("%d%s%d", &from, cmdt, &to);
			if(pos[from] != pos[to]){
				if(!strcmp(cmdf, "move"))
					init(pos[from], from);
				if(!strcmp(cmdt, "onto"))
					init(pos[to], to);
				move_top(pos[from], from, pos[to]);
			}
		}
	}
	return 0;
}
