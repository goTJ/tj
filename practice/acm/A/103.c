/*   @JUDGE_ID:   10319NX   103   C */
#include<stdio.h>
#include<stdlib.h>

typedef struct{
	int item;
	int box[10];
}box_type;

box_type many[30];
int max_num, max_length;
int done[30];
int in[30];

int comp(void const *p, void const *q)
{
	return *(int *)q - *(int *)p;
}

int compp(void const *p, void const *q)
{
	return ((box_type *)p)->box[0] - ((box_type *)q)->box[0];
}

int can_put(int big, int small)
{
	int i;

	for(i=0; i<max_length; i++)
		if(many[big].box[i] >= many[small].box[i])
			return 0;
	return 1;
}

int test(int n)
{
	int i, j, sum = 1, t;

	if(!done[n]){
		done[n] = 1;
		for(i=n+1; i<max_num; i++){
			if(can_put(n, i) && (t=test(i)+1) > sum){
				sum = t;
			}
		}
		in[n] = sum;
	}
	else
		sum = in[n];
	return sum;
}

int jjj(int nnn)
{
	int i;

	for(i=0; i<max_num; i++)
		if(many[i].item == nnn)
			return i;
}

void output_way(int max, int nn)
{
	int min_way = 30;
	int i, final;

	if(max){
		if(nn != 31)
			final = jjj(nn);
		for(i=0; i<max_num; i++)
			if(in[i]==max && many[i].item<min_way 
					&& (nn==31||can_put(final, i))){
				min_way = many[i].item;
			}
		printf("%d ", min_way+1);
		output_way(max-1, min_way);

	}
}

int main(void)
{
	int i, j, max, t;

	while(scanf("%d%d", &max_num, &max_length) != EOF){
		memset(in, 0, sizeof(in));
		memset(done, 0, sizeof(done));
		max = 0;
		for(i=0; i<max_num; i++){
			many[i].item = i;
			for(j=0; j<max_length; j++)
				scanf("%d", &(many[i].box[j]));
		}
		for(i=0; i<max_num; i++)
	  	   qsort(many[i].box, max_length, sizeof(many[0].box[0]), comp);
		qsort(many, max_num, sizeof(many[0]), compp);
		for(i=0; i<max_num; i++)
			if((t=test(i)) > max)
				max = t;
		printf("%d\n", max);
		output_way(max, 31);
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
