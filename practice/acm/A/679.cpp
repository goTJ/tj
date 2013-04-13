#include<stdio.h>

int main()
{
	int round, times;
	int num, i;
	int depth;
	int value;

	scanf("%d", &times);
	for(round=0; round<times; round++){
		scanf("%d%d", &depth, &num);
		num--;
		value = 1;
		for(i=1; i<depth; i++){
			value = value*2+(num%2);
			num /= 2;
		}
		printf("%d\n", value);
	}
	scanf("%d", &times);
	return 0;
}
