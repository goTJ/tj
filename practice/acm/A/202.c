#include<stdio.h>
#include<string.h>

int repeat[3003];
int num[52];

void initial(void)
{
	memset(repeat, 0, sizeof(repeat));
}

int find(int num, int a)
{
	int sum=0;

	if(!num)
		return 0;
	while(!(num%a)){
		sum++;
		num /= a;
	}
	return sum;
}

int main(void)
{
	int a, b, n=1;
	int t, i, j;
	int two, five, pos, nott;
	int times=0;

	while(scanf("%d%d", &a, &b) != EOF){
		initial();
		if(times)
			printf("\n");
		n = 1;
		pos = 50;
		printf("%d/%d = %d.", a, b, a/b);
		two = find(b, 2)-find(a, 2);
		five = find(b, 5)-find(a, 5);
		nott = (two>five) ? two : five;
		a = (a%b)*10;
		if(a){
			for(i=0; i<nott; i++){
				printf("%d", a/b);
				a = (a%b)*10;
			}
		}
		printf("(");
		while(1){
			t = a%b;
			if(repeat[t])
				break;
			if(n <= pos)
				printf("%d", a/b);
			a = t*10;
			repeat[t] = n;
			n++;
		}
		if(n > pos+1)
			printf("...");
		printf(")");
		printf("\n");
		printf("\t%d = number of digits in repeating cycle\n", n-1);
		times++;
	}
	return 0;
}
