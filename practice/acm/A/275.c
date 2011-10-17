/* @JUDGE_ID:   10319NX 275 C */
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
	int two, five, not, pos;
	int times=0;

	while(scanf("%d%d", &a, &b) != EOF){
		if(!a && !b)
			break;
		initial();
		if(times)
			printf("\n");
		two = find(b, 2)-find(a, 2);
		five = find(b, 5)-find(a, 5);
		not = (two>five) ? two : five;
		printf(".");
		n = pos = 1;
		a = (a%b)*10;
		if(a){
			for(i=0; i<not; i++){
				printf("%d", a/b);
				pos++;
				a = (a%b)*10;
			}
		}
		if(a){
			while(1){
				t = a%b;
				if(repeat[t])
					break;
				if(!(pos%50))
					printf("\n");
				printf("%d", a/b);
				pos++;
				n++;
				a = t*10;
				repeat[t] = n;
			}
		}
		printf("\n");
		if(a)
			printf("The last %d digits repeat forever.\n", n-1);
		else
			printf("This expansion terminates.\n");
		times++;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
