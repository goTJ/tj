#include<stdio.h>

int main()
{
	int n;
	while(scanf("%d", &n) == 1){
		if(n == 0){
			printf("0\n");
		}else{
			int remain=0;
			int count=0;
			do{
				remain = (remain*10+1)%n;
				count++;
			}while(remain);
			printf("%d\n", count);
		}
	}
	return 0;
}
