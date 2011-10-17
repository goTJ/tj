#include<cstdio>

int main()
{
	int round;
	scanf("%d", &round);
	for(int times=0; times<round; times++){
		bool hartal[4000]={0};
		int lim;
		scanf("%d", &lim);
		int n;
		scanf("%d", &n);
		int sum=0;
		for(int i=0; i<n; i++){
			int factor;
			scanf("%d", &factor);
			for(int val=factor; val<=lim; val+=factor){
				if(!hartal[val] && (val%7)!=6 && (val%7)!=0){
					sum++;
					hartal[val] = true;
				}
			}
		}
		printf("%d\n", sum);
	}
	return 0;
}
