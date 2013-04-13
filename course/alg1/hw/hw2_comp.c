#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef float Type;

int comp(const void *p, const void *q)
{
	if(*(Type *)p > *(Type *)q)
		return 1;
	else
		return -1;
}

int main()
{
	clock_t begin, end;
	clock_t clk1, clk2;
	Type *data;
	int n, i;
	freopen("test_data.txt", "r", stdin);
	FILE *fout=fopen("result_ans.txt", "w");
	begin = clock();

clk1 = clock();
	scanf("%d", &n);
	data = (Type *)malloc(sizeof(Type)*n);
	for(i=0; i<n; i++)
		scanf("%f", &data[i]);
clk2 = clock();
printf("time = %.3f sec\n", (double)(clk2-clk1)/CLOCKS_PER_SEC);
clk1 = clock();
	qsort(data, n, sizeof(Type), comp);
clk2 = clock();
printf("time = %.3f sec\n", (double)(clk2-clk1)/CLOCKS_PER_SEC);
clk1 = clock();
/*
	fprintf(fout, "%d\n", n);
	for(i=0; i<n; i++){
		if((i%10) == 9)
			fprintf(fout, "%10.3f\n", data[i]);
		else
			fprintf(fout, "%10.3f ", data[i]);
	}
	*/
	free(data);
	fclose(fout);
clk2 = clock();
printf("time = %.3f sec\n", (double)(clk2-clk1)/CLOCKS_PER_SEC);

	end = clock();
	printf("time = %.3f sec\n", (double)(end-begin)/CLOCKS_PER_SEC);
	return 0;
}
