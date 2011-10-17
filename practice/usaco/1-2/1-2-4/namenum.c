//### PROGRAM
/*
id: flyherm003
prog: namenum
*/
#include<stdio.h>
#include<stdlib.h>

typedef struct hermit{
	char name[20];
	struct hermit *next;
}cow;

typedef struct fly{
	struct fly *next[10];
	cow *node;
}tree;

tree *dict;
int sign[258];

void add_name(tree *dict, char s[])
{
	cow *n, *p;

	n = (cow *)malloc(sizeof(cow));
	sscanf(s, "%s", n->name);
	n->next = dict->node;
	dict->node = n;
}

tree *add_node(void)
{
	int i;
	tree *p;
	
	p = (tree *)malloc(sizeof(tree));
	p->node = NULL;
	for(i=0; i<10; i++)
		p->next[i] = NULL;
	return p;
}

void hash(tree *dict, char tmp[], int pos)
{
	int mode;

	if(tmp[pos]){
		mode = sign[tmp[pos]];
		if(dict->next[mode] == NULL){
			dict->next[mode] = add_node();
		}
		hash(dict->next[mode], tmp, pos+1);
	}
	else{
		add_name(dict, tmp);
	}
}

void initial(void)
{
	FILE *in=fopen("dict.txt", "r");
	char tmp[20];
	int i;

	sign['A'] = sign['B'] = sign['C'] = 2;
	sign['D'] = sign['E'] = sign['F'] = 3;
	sign['G'] = sign['H'] = sign['I'] = 4;
	sign['J'] = sign['K'] = sign['L'] = 5;
	sign['M'] = sign['N'] = sign['O'] = 6;
	sign['P'] = sign['R'] = sign['S'] = 7;
	sign['T'] = sign['U'] = sign['V'] = 8;
	sign['W'] = sign['X'] = sign['Y'] = 9;
	dict = add_node();
	while(fscanf(in, "%s", tmp) != EOF){
		hash(dict, tmp, 0);
	}
}

void output(cow *node)
{
	if(node){
		output(node->next);
		printf("%s\n", node->name);
	}
}


void search(tree *dict, char in[], int pos)
{
	if(dict == NULL)
		printf("NONE\n");
	else if(in[pos]){
		search(dict->next[in[pos]], in, pos+1);
	}
	else{
		output(dict->node);
	}
}

int main(void)
{
	char in[20];
	int i;
	freopen("namenum.in", "r", stdin);
	freopen("namenum.out", "w", stdout);

	initial();
	while(scanf("%s", in) != EOF){
		i = 0;
		while(in[i])
			in[i++] -= '0';
		search(dict, in, 0);
	}
	return 0;
}
//### END
