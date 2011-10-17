#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"mysort.h"

#define NUM 4

int comp(const void *p, const void *q)
{
	return *(int *)p-*(int *)q;
}

int main()
{
	int round, times=8;
	int num=10;
	void (*func[NUM])(void *, size_t , size_t , int(*)(const void *, const void *))={qsort, myqsort2, merge_sort, qsort1};
	int *data[NUM];
	int i, j;

	srand(time(NULL));
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
			int tmp = (int)((double)rand()/RAND_MAX*num);
			for(j=0; j<NUM; j++)
				data[j][i] = tmp;
		}

		for(i=0; i<NUM; i++){
			int begin, end;
			begin = clock();
			(*func[i])(data[i], num, sizeof(int), comp);
			end = clock();
			printf("%5d  ", (end-begin)/10000);
			if(i){
				for(j=0; j<num; j++){
					if(data[i][j] != data[0][j]){
						printf("F ");
						break;
					}
				}
				if(j == num)
					printf("T ");
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
