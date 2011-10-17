#include<stdio.h>
#include<stdlib.h>

int main()
{
	bool a[3000];
	int n;
	bool isJolly;
	int last, now;

	while(scanf("%d", &n) == 1){
		for(int i=0; i<n; i++)
			a[i] = false;
		scanf("%d", &last);
		for(int i=1; i<n; i++){
			scanf("%d", &now);
			a[abs(now-last)] = true;
			last = now;
		}
		isJolly = true;
		for(int i=1; i<=n-1; i++){
			if(!a[i]){
				isJolly = false;
				break;
			}
		}
		if(isJolly)
			printf("Jolly\n");
		else
			printf("Not jolly\n");
	}
	return 0;
}
