// vim:ft=c:foldmethod=marker
#include<stdio.h>
#include<stdlib.h>

#define MAX_NUM 262144

typedef struct Node{
	int length;
	int first;
	int last;
}Node;

typedef struct Data{
	int num;
	int idx;
}Data;

Data data[MAX_NUM];
int in[MAX_NUM];
Node heap[MAX_NUM*2]={0};
int num=0;
int offset=1;

int get_num(int *np)
{
	int c;
	int n=0;

	while((c=getchar()) != EOF){
		if(isdigit(c))
			break;
	}
	if(c == EOF)
		return 0;
	n = (c&15);
	while((c=getchar()) != EOF){
		if(!isdigit(c))
			break;
		n = n*10+(c&15);
	}
	*np = n;
	return 1;
}

int comp(const void *p, const void *q)
{
	int t=((Data *)p)->num-((Data *)q)->num;

	return t ? t : ((Data *)q)->idx-((Data *)p)->idx;
}

void output3(int i, int n)
{
	if(i == -1 || n == 3)
		return;
	output3(heap[i+offset].last, n+1);
	printf("%d ", in[i]);
}

int main()
{
	int tmp;
	int i, j;

	while(get_num(&tmp) == 1){
		in[num] = tmp;
		data[num].num = tmp;
		data[num].idx = num;
		num++;
	}
	while(offset < num)
		offset <<= 1;
	qsort(data, num, sizeof(Data), comp);
	for(i=0; i<num; i++){
		int max = 0;
		int first;
		int last;
		for(j=data[i].idx+offset; j!=0; j>>=1){
			if(j&1 && heap[j-1].length > max){
				max = heap[j-1].length;
				if(j >= offset){
					last = j-1-offset;
				}else{
					first = heap[j-1].first;
					last = heap[j-1].last;
				}
			}
		}
		max++;
		j = data[i].idx+offset;
		heap[j].length = max;
		heap[j].last = (max == 1) ? -1 : last;
		for(j>>=1; j!=0; j>>=1){
			if(heap[j].length >= max)
				break;
			heap[j].length = max;
			if(max <= 3)
				heap[j].first = data[i].idx;
			heap[j].last = data[i].idx;
		}
	}
	printf("max length: %d\n", heap[1].length);
	printf("first three: ");
	output3(heap[1].first, 0);
	printf("\n");
	printf("last three: ");
	output3(heap[1].last, 0);
	printf("\n");
	return 0;
}
