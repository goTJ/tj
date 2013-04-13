/*   @JUDGE_ID:   10319NX   105   C */
#include<stdio.h>

int map[20000];
int done = 0;
int ori = 0;

void build(int left, int right, int high)
{
	int i;

	for(i=left+10000; i<right+10000; i++)
		if(map[i] < high)
			map[i] = high;
}

void draw(int end)
{
	int i;

	for(i=done; i<end; i++)
		if(map[i] != ori){
			printf("%d %d ", i-10000, map[i]);
			ori = map[i];
		}
	done = i;
}

int main(void)
{
	int i;
	int left_new, high_new, right_new;

	for(i=0; i<20000; i++)
		map[i] = 0;
	while(scanf("%d%d%d", &left_new, &high_new, &right_new) == 3){
		build(left_new, right_new, high_new);
		draw(left_new);
	}
	draw(20000);
	printf("\n");
	return 0;
}
/* @END_OF_SOURCE_CODE */
