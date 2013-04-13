/*###########################################################################*/
/*       title:                            Rational                          */
/*                                                                           */
/*           start: */
/*                                                                           */
/*                finish: */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   493   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>

unsigned int times;

unsigned int gcd(unsigned int a, unsigned int b)
{
	unsigned int t;

	while((t=a%b)){
		a = b;
		b = t;
	}
	return b;
}

void got(unsigned int num, unsigned int fly, unsigned int sum)
{
	unsigned int ttt, kkk, i, t;
	char p[2] = { '\0', '\0' };

	fly--;
	ttt = fly/(sum/4);
	kkk = fly%(sum/4);
	kkk++;
	if(ttt==3 || ttt==0)
		p[0] = '-';
	if(ttt==1 || ttt==3){
		t = 1;
		for(i=1; i<num&&kkk; i++){
			if(gcd(i, num) == 1){
				t = i;
				kkk--;
			}
		}
	}
	if(ttt==2 || ttt==0){
		t = num-1;
		for(i=num-1; i>0&&kkk; i--){
			if(gcd(i, num) == 1){
				kkk--;
				t = i;
			}
		}
	}
	if(ttt <= 1)
		printf("%s%d / %d\n", p, num, t);
	if(ttt >= 2)
		printf("%s%d / %d\n", p, t, num);
}

int find(unsigned int num)
{
	unsigned int sum=0;
	unsigned int i, j;

	for(i=1; i<num; i++)
		if(gcd(i, num) == 1){
			sum++;
		}
	sum *= 4;
	if(times > sum){
		times -= sum;
		return 1;
	}
	else{
		got(num, times, sum);
		return 0;
	}
}

int main(void)
{
	unsigned int i;

	while(scanf("%u", &times) != EOF){
		if(times==0)
			printf("1 / 1\n");
		else if(times==1)
			printf("0 / 1\n");
		else if(times==2)
			printf("-1 / 1\n");
		else{
			times -= 2;
			for(i=2;  find(i); i++);
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
