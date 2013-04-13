// vim:ft=c:foldmethod=marker
#include<stdio.h>
#include<time.h>

#define MAX_NUM 500000

typedef struct Node{
	int max_val;
	int last_idx;
	int third_idx;
}Node;

typedef struct Data{
	int val;
	int from;
}Data;

Node table[MAX_NUM];
Data data[MAX_NUM];
int max_len=0;
FILE *fin;

int get_num(int *np)
{
	int c;
	int n=0;

	while((c=fgetc(fin)) != EOF){
		if(isdigit(c))
			break;
	}
	if(c == EOF)
		return 0;
	n = (c&15);	// n = c-'0';
	while((c=fgetc(fin)) != EOF){
		if(!isdigit(c))
			break;
		n = n*10+(c&15);
	}
	*np = n;
	return 1;
}

int bin_search(int val)
{
	int u=max_len-1, l=0, mid=(l+u)>>1;

	while(l <= u){
		mid = (l+u)>>1;	// mid = (l+u)/2
		if(val <= table[mid].max_val)
			u = mid-1;
		else if(val > table[mid].max_val)
			l = mid+1;
	}
	return (val > table[mid].max_val) ? mid+1 : mid;
}

void outputn(int idx, int n)
{
	if(!n || idx==-1)
		return;
	outputn(data[idx].from, n-1);
	printf("%d ", data[idx].val);
}

int main()
{
	int val, pos;
	int i;
	clock_t begin;
	fin=fopen("input.txt", "r");

	begin = clock();
	for(i=0; get_num(&val); i++){
		data[i].val = val;
		pos = bin_search(val);
		table[pos].max_val = val;
		if(pos == max_len)
			max_len++;
		table[pos].last_idx = i;
		table[pos].third_idx = (pos < 3) ? i : table[pos-1].third_idx;
		data[i].from = (pos) ? table[pos-1].last_idx : -1;
	}
	printf("max length: %d\n", max_len);
	printf("biggest three: ");
	outputn(table[max_len-1].last_idx, 3);
	printf("\n");
	printf("smallest three: ");
	outputn(table[max_len-1].third_idx, 3);
	printf("\n");
	printf("time: %.3lf sec\n", (double)(clock()-begin)/CLOCKS_PER_SEC);
	fclose(fin);
	return 0;
}
