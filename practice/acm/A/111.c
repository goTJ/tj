#include<stdio.h>

int tab[25];
int num[25];
int len[25];
int lim;

void initial(void)
{
	int i;

	for(i=0; i<lim; i++)
		len[i] = 1;
}

int main(void)
{
	int i, j, t;
	int sum;

	scanf("%d", &lim);
	for(i=0; i<lim; i++){
		scanf("%d", &t);
		tab[i] = t-1;
	}
	while(1){
		initial();
		sum = 0;
		for(i=0; i<lim; i++){
			if(scanf("%d", &t) == EOF)
				return 0;
			num[t-1] = tab[i];
		}
		for(i=0; i<lim; i++){
			for(j=0; j<i; j++){
				if(num[j]<num[i] && len[j]+1>len[i])
					len[i] = len[j]+1;
			}
		}
		t = len[1];
		for(i=2; i<=lim; i++)
			if(len[i] > t)
				t = len[i];
		sum += t;
		printf("%d\n", sum);
	}
	return 0;
}
