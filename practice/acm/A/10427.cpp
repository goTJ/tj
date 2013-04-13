/* @JUDGE_ID:   10319NX 10427 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>

int main(void)
{
	int index;
	int step, num;
	int n, d, sum;
	char buf[100000];

	while(scanf("%d", &index) == 1){
		step = 1;
		num = 9;
		n = 0;
		sum = 0;
		while(n < index){
			n += step*num;
			sum += num;
			step++;
			num *= 10;
		}
		step--;
		num /= 10;
		n -= step*num;
		sum -= num;
//		printf("%d %d %d\n", step, sum, n);
		d = index-n-1;
		sum += (d/step)+1;
		sprintf(buf, "%d", sum);
//		printf("%d: %d -> %d\n", index, sum, d%step);
		printf("%c\n", buf[d%step]);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
