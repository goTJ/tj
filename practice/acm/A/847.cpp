#include<cstdio>

int main()
{
	int n;
	while(scanf("%d", &n) == 1){
		bool flag=true;
		while(n>1){
			int val=(flag) ? 9 : 2;
			n = (n-1)/val+1;
			flag = !flag;
		}
		printf(flag ? "Ollie wins.\n" : "Stan wins.\n");
	}
	return 0;
}
