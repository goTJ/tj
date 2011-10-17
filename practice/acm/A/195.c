#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

int tab[256];

void initial(void)
{
	int i;

	for(i=0; i<26; i++){
		tab['A'+i] = i*2;
		tab['a'+i] = i*2+1;
	}
}

void swap(char *a, char *b)
{
	char t;

	t = *a;
	*a = *b;
	*b = t;
}

int comp(const void *p, const void *q)
{
	return tab[*(char *)p] - tab[*(char *)q];
}

int find_next(char in[], int begin, int end, int val)
{
	int i;
	int max=100, pos;

	for(i=begin; i<end; i++){
		if(tab[in[i]]>val && tab[in[i]]<max){
			max = tab[in[i]];
			pos = i;
		}
	}
	return pos;
}

int main(void)
{
	int round, times;
	char in[1000];
	int len;
	int i, j;
	int max, t;
	
	initial();
	scanf("%d", &round);
	for(times=0; times<round; times++){
		scanf("%s", in);
		len = strlen(in);
		qsort(in, len, sizeof(in[0]), comp);
		printf("%s\n", in);
		while(1){
			for(i=len-2; i>=0; i--){
				if(tab[in[i]] < tab[in[i+1]]){
					t = find_next(in, i+1, len, tab[in[i]]);
					swap(&in[i], &in[t]);
					qsort(in+i+1, len-i-1, sizeof(in[0]), comp);
					break;
				}
			}
			if(i == -1)
				break;
			printf("%s\n", in);
		}
	}
	return 0;
}
