#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"mysort.h"

#define NUM 2

int comp(const void *p, const void *q)
{
	return *(int *)p - *(int *)q;
}

void swap(int *a, int *b)
{
	int tmp;

	if(a == b)
		return;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

int find_kth_by_mid(int data[], int n, int k)
{
	if(k >= n)
		return -1;
	if(n <= 5){
		insertion_sort(data, n, sizeof(int), comp);
		return data[k];
	}else{
		int i;
		int radix;
		int head, tail;
		int flag=1;
		for(i=4; i<n; i+=5)
			insertion_sort(data+i-4, 5, sizeof(int), comp);
		for(i=4; i<n; i+=5)
			swap(data+(i/5), data+i-2);
		radix = find_kth_by_mid(data, n/5, n/10);
		head = -1, tail = n;
		while(1){
			while(data[++head] <= radix){
				if(data[head] == radix && flag){
					swap(data+head, data+n-1);
					flag = 0;
					if(data[head] > radix)
						break;
				}
			}
			while(data[--tail] >= radix){
				if(data[tail] == radix){
					swap(data+tail, data+n-1);
					flag = 0;
				}
			}
			if(head >= tail)
				break;
			swap(data+head, data+tail);
		}
		swap(data+head, data+n-1);
		if(k > head){
			return find_kth_by_mid(data+head+1, n-head-1, k-head-1);
		}else if(k < head){
			return find_kth_by_mid(data, head, k);
		}else{
			return data[head];
		}
	}
}

int find_kth_by_qsort(int data[], int n, int k)
{
	qsort(data, n, sizeof(int), comp);
	return data[k];
}

int find_kth_by_trivial(int data[], int n, int k)
{
	int i, j;

	for(i=0; i<=k; i++){
		int minp=i;
		for(j=i+1; j<n; j++){
			if(data[j] < data[minp])
				minp = j;
		}
		swap(data+i, data+minp);
	}
	return data[k];
}

int main()
{
	int round, times=9;
	int num=10;
	int i, j;
	int (*func[NUM])(int *, int, int)={find_kth_by_qsort, find_kth_by_mid};
	int *data[NUM];
	int ans[NUM];
	int k;

	srand(time(0));
	printf("    num  ");
	for(i=0; i<NUM; i++){
		printf("func%d  ", i);
		if(i)
			printf("  ");
	}
	printf("\n");
	for(round=0; round<times; round++){
		printf("%7d  ", num);
		for(i=0; i<NUM; i++)
			data[i] = (int *)malloc(sizeof(int)*num);
		for(i=0; i<num; i++){
			data[0][i] = ((double)rand()/RAND_MAX)*num;
			for(j=1; j<NUM; j++)
				data[j][i] = data[0][i];
		}
		k = ((double)rand()/RAND_MAX)*num;
		for(i=0; i<NUM; i++){
			int begin, end;
			begin = clock();
			ans[i] = (*func[i])(data[0], num, k);
			end = clock();
			printf("%5d  ", (end-begin)/10000);
			if(i){
				if(ans[i] == ans[0])
					printf("T ");
				else
					printf("F ");
			}
		}
		printf("\n");

		for(i=0; i<NUM; i++)
			free(data[i]);
		if(num < 1000000)
			num *= 10;
		else
			num += 1000000;

	}
	return 0;
}
