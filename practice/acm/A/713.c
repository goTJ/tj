/* @JUDGE_ID:   10319NX 713 C */
#include<stdio.h>
#include<string.h>

void reverse(char a[], int len)
{
	int i;
	char tmp;

	for(i=0; i<len/2; i++){
		tmp = a[i];
		a[i] = a[len-i-1];
		a[len-i-1] = tmp;
	}
}

int main(void)
{
	int round, times;
	char a[20], b[20];
	int len_a, len_b;
	int va, vb;

	scanf("%d", &round);
	for(times=0; times<round; times++){
		scanf("%s%s", a, b);
		len_a = strlen(a);
		len_b = strlen(b);
		reverse(a, len_a);
		reverse(b, len_b);
		sscanf(a, "%d", &va);
		sscanf(b, "%d", &vb);
		sprintf(a, "%d", va+vb);
		len_a = strlen(a);
		reverse(a, len_a);
		sscanf(a, "%d", &va);
		printf("%d\n", va);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
