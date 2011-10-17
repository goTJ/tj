#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define STACK_NUM 32
#define STOP_POINT 9
#define push(h, t)	{stack[sp].head=h; stack[sp].tail=t; sp++;}
#define pop(h, t)	{sp--; h=stack[sp].head; t=stack[sp].tail;}
#define swap(a, b)	{tmp=a, a=b, b=tmp; }

typedef float Type;

typedef struct Node{
	Type *head;
	Type *tail;
}Node;

void myqsort(Type data[], int n)
{
	static Node stack[STACK_NUM];
	int sp=1;	// stack head position
	Type *head=data, *tail=data+n-1, *mid;
	Type *hp, *tp;
	int len1,len2;
	Type tmp;
	int i, j;

	while(sp > 0){
		// mid of three
		mid = head+(tail-head)/2;
		if(*head < *mid){
			if(*head < *tail){
				if(*mid < *tail)
					swap(*mid, *tail);
			}else
				swap(*head, *tail);
		}else{
			if(*mid < *tail){
				if(*head < *tail)
					swap(*head, *tail);
			}else
				swap(*mid, *tail);
		}
		// partition
		hp = head, tp=tail-1;
		while(1){
			while(*hp <= *tail)
				hp++;
			while(*tp >= *tail)
				tp--;
			if(hp >= tp)
				break;
			swap(*hp, *tp);
		}
		swap(*hp, *tail);
		hp++;
		// judge two segment
		len1 = tp-head+1;
		len2 = tail-hp+1;
		if(len1 <= 5){
			if(len2 <= 5){
				pop(head, tail);
			}else{
				head = hp;
			}
		}else{
			if(len2 <= 5){
				tail = tp;
			}else if(len1 < len2){
				push(hp, tail);
				tail = tp;
			}else{
				push(head, tp);
				head = hp;
			}
		}
	}
	// insertion sort
	for(i=0; i<n; i++){
		tmp = data[i];
		for(j=i-1; j>=0; j--){
			if(tmp < data[j]){
				data[j+1] = data[j];
			}else
				break;
		}
		data[j+1] = tmp;
	}
}

int main()
{
	clock_t begin, end;
	Type *data;
	int n, i;
	freopen("test_data.txt", "r", stdin);
	FILE *fout=fopen("result.txt", "w");

	scanf("%d", &n);
	data = (Type *)malloc(sizeof(Type)*n);
	for(i=0; i<n; i++)
		scanf("%f", &data[i]);
	begin = clock();
	myqsort(data, n);
	end = clock();
	fprintf(fout, "%d\n", n);
	for(i=0; i<n; i++){
		if((i%10) == 9)
			fprintf(fout, "%10.3f\n", data[i]);
		else
			fprintf(fout, "%10.3f ", data[i]);
	}
	free(data);
	fclose(fout);

	printf("time = %.3f sec\n", (double)(end-begin)/CLOCKS_PER_SEC);
	return 0;
}
