#include<stdio.h>
#include<string.h>

int main(void)
{
	int s[20000]={0};
	int len;
	int n;
	int i, j;
	int sum;

	while(scanf("%d", &n) == 1){
		s[0] = 1;
		len = 1;
		s[len] = 0;
		for(i=2; i<=n; i++){
			for(j=0; j<len; j++)
				s[j] *= i;
			for(j=0; j<len; j++){
				if(s[j] >= 10){
					s[j+1] += s[j]/10;
					s[j] %= 10;
				}
				while(s[len]){
					len++;
					s[len] = 0;
				}
			}
		}
		for(j=0,sum=0; j<len; j++)
			sum += s[j];
		printf("%d\n", sum);
	}
	return 0;
}
