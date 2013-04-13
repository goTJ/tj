/* @JUDGE_ID:   10319NX 10161 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>

int main(void)
{
	int n, i;
	int val, flag;
	int row, col;

	while(scanf("%d", &val) != EOF){
		if(!val)
			break;
		for(i=1,n=1; n<val;i++,n=i*i)
			;
		flag = i&1;
		row = 1;
		col = i;
		val = n-val;
		if(val/i){
			row = i;
			val -= i-1;
			col -= val%i;
		}
		else
			row += val%i;
		if(flag)
			printf("%d %d\n", row, col);
		else
			printf("%d %d\n", col, row);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
