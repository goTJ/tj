/*   @JUDGE_ID:   10319NX   10022   C */
#include<stdio.h>

typedef struct{
	int flr;
	int num;
}pos;
int step;

int max_n(int a, int b)
{
	return (a>b) ? a : b;
}

int min_n(int a, int b)
{
	return (a<b) ? a : b;
}

void way(int n, pos *put)
{
	int i;

	for(i=1; i*i<n; i++);
	put->flr = i;
	put->num = (2*i-1)-(i*i-n);
	/*(printf("%d %d\n", put->flr, put->num);*/
}

int can_down(pos *a)
{
	return (a->num)%2 ? 1 : 0;
}

void walk(pos *from, pos *to)
{
	int i;

	while(from->flr < to->flr){
		if(can_down(from)){
			from->flr++;
			from->num++;
			step++;
		}
		else{
			if(from->num > to->num){
				from->num--;
				step++;
			}
			else{
				from->num++;
				step++;
			}
		}
	}
	step += max_n(from->num, to->num) - min_n(from->num, to->num);
}

int main(void)
{
	int time;
	int a, b;
	int max, min;
	int i;
	pos from, to;

	scanf("%d", &time);
	for(i=0; i<time; i++){
		step = 0;
		scanf("%d%d", &a, &b);
		max = max_n(a, b);
		min = min_n(a, b);
		way(min, &from);
		way(max, &to);
		walk(&from, &to);
		printf("%d\n\n", step);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
