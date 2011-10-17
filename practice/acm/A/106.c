/*   @JUDGE_ID:   10319NX   106   C */
#include<stdio.h>
#include<string.h>

int used[1000002];
int lim = 1;
unsigned int num;
unsigned int nouse;
unsigned int times;

int prime(unsigned int max, unsigned int min)
{
	unsigned int tmp = 1;

	while(tmp){
		tmp = max%min;
		max = min;
		min = tmp;
	}
	if(max == 1)
		return 1;
	else
		return 0;
}

void use(unsigned int i, unsigned int j, unsigned int z) 
{
	unsigned int n;
	
	for(n=1; z*n<=num; n++){
		if(!used[i*n]){
			nouse--;
			used[i*n] = 1;
		}
		if(!used[j*n]){
			nouse--;
			used[j*n] = 1;
		}
		if(!used[z*n]){
			nouse--;
			used[z*n] = 1;
		}
	}
}

int main(void)
{
	unsigned int i, j, x, y, z;

	while(scanf("%u", &num) != EOF){
		times = 0;
		nouse = num;
		memset(used, 0, sizeof(used));
		for(i=1; i*i*2<=num; i++)
			for(j=i+1; i*i+j*j<=num; j++){
				x = j*j-i*i;
				y = i*j*2;
				z = i*i+j*j;
				if(prime(x, y)&&prime(x,z)&&prime(y,z)){
					times++;
					use(x, y, z);
				}
			}
		printf("%u %u\n", times, nouse);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
