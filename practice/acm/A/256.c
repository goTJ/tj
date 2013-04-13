/*###########################################################################*/
/*       title: Quirksome Squares                                            */
/*                                                                           */
/*           start: 2001年  7月24日 周二 17時19分16秒 CST                    */
/*                                                                           */
/*                finish: 2001年  7月24日 周二 17時30分51秒 CST              */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   256   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>

void output(unsigned int a, unsigned int b, unsigned int nnn)
{
	unsigned int i, n, tmp;

	for(tmp=a,n=0; tmp>0; tmp/=10,n++);
	if(!n)
		n++;
	for(i=n; i<nnn; i++)
		printf("0");
	printf("%u", a);
	for(tmp=b,n=0; tmp>0; tmp/=10,n++);
	if(!n)
		n++;
	for(i=n; i<nnn; i++)
		printf("0");
	printf("%u\n", b);
}

int main(void)
{
	unsigned int val, n, i, max, fly;

	while(scanf("%u", &n) != EOF){
		max = fly = 1;
		for(i=0; i<n; i++)
			max *= 10;
		for(i=0; i<n/2; i++)
			fly *= 10;
		for(i=val=0; val<max; i++,val=i*i)
			if(i == val/fly+val%fly)
				output(val/fly, val%fly, n/2);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
