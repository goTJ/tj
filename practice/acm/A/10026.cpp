#include<stdio.h>

int main()
{
	int round;
	scanf("%d", &round);
	for(int times=0; times<round; times++){
		double ave[1006];
		int n;
		scanf("%d", &n);
		for(int i=0; i<n; i++){
			int time, fine;
			scanf("%d%d", &time, &fine);
			ave[i] = (double)fine/time;
		}
		for(int i=0; i<n; i++){
			int maxp=0;
			for(int j=1; j<n; j++){
				if(ave[j] > ave[maxp])
					maxp = j;
			}
			if(i)
				printf(" ");
			printf("%d", maxp+1);
			ave[maxp] = -1.0;
		}
		printf("\n");
	}
	return 0;
}
