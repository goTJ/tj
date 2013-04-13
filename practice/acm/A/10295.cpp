#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Word{
	char word[17];
	int value;
	struct Word *next;
};

Word *tab[26];

int main()
{
	int M, N;
	int i;
	char buf[20];
	int val;
	int index;
	Word *node, *p;
	int sum=0;
	int flag;

	scanf("%d%d", &M, &N);
	for(i=0; i<M; i++){
		node = (Word *)malloc(sizeof(Word));
		scanf("%s%d", node->word, &(node->value));
		node->next = NULL;
		index = node->word[0]-'a';
		if(tab[index]){
			if(strcmp(node->word, tab[index]->word) < 0){
				node->next = tab[index];
				tab[index] = node;
			}
			else{
				p = tab[index];
				if(p->next && strcmp(node->word, p->next->word) > 0){
					p = p->next;
				}
				node->next = p->next;
				p->next = node;
			}
		}
		else{
			tab[index] = node;
		}
	}
	while(scanf("%s", buf) != EOF){
		if(buf[0] == '.'){
			printf("%d\n", sum);
			sum = 0;
		}
		else{
			index = buf[0]-'a';
			p = tab[index];
			flag = -1;
			while(p && (flag=strcmp(buf, p->word)) > 0)
				p = p->next;
			if(flag == 0)
				sum += p->value;
		}
	}
	return 0;
}
