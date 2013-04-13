/*   @JUDGE_ID:   10319NX   146   C */
#include<stdio.h>
#include<string.h>

char input[52];
int length;

int find(void)
{
	int i = length-1;
	char c = input[length-1];

	while(i>=0&&input[i--]>=c)
		c = input[i+1];
	if(input[i+1]>=c)
		i--;
	return i+1;
}

int comp(const void *p, const void *q)
{
	return *(char *)p - *(char *)q;
}

void swap(int a, int b)
{
	char tmp;

	tmp = input[a];
	input[a] = input[b];
	input[b] = tmp;
}

void next(int pos)
{
	char c=input[pos], min;
	int i, j;

	for(i=pos+1; i<length; i++){
		if(input[i]<=c)
			break;
		else
			min = input[i];
	}
	swap(pos, i-1);
	qsort(&(input[pos+1]), length-pos-1, sizeof(input[0]), comp);
}

int main(void)
{
	int pos;

	while(scanf("%s", input) != EOF){
		length = strlen(input);
		if(length==1 && input[0]=='#')
			exit(0);
		pos = find();
		if(pos == -1)
			printf("No Successor\n");
		else{
			next(pos);
			printf("%s\n", input);
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
