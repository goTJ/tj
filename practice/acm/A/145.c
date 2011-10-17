/* @JUDGE_ID:   10319NX 145 C */
#include<stdio.h>
#include<string.h>

double money[5][3] = { 0.10, 0.06, 0.02,
		       0.25, 0.15, 0.05,
		       0.53, 0.33, 0.13,
		       0.87, 0.47, 0.17,
		       1.44, 0.80, 0.30 };
int time[24];

void initial(void)
{
	int i;

	for(i=0; i<8; i++)
		time[i] = 2;
	for(i=8; i<18; i++)
		time[i] = 0;
	for(i=18; i<22; i++)
		time[i] = 1;
	for(i=22; i<24; i++)
		time[i] = 2;
}

int circle(int n)
{
	return (n+1)%24;
}

int main(void)
{
	int i, type;
	char c, s[11];
	int hh, mm, h, m;
	int cost[3];
	double sum=0.0;

	initial();
	while((c=getchar()) != EOF){
		sum = 0.0;
		memset(cost, 0, sizeof(cost));
		if(c == '#')
			break;
		type = c-'A';
		scanf(" %s ", s);
		scanf("%d %d %d %d\n", &h, &m, &hh, &mm);
		if(h==hh && mm>m)
			cost[time[h]] += mm-m;
		else{
			cost[time[h]] += 60-m;
			for(i=circle(h); i!=hh; i=circle(i))
				cost[time[i]] += 60;
			cost[time[hh]] += mm;
		}
		for(i=0; i<3; i++)
			sum += (double)(cost[i]*money[type][i]);
		printf("%10s", s);
		for(i=0; i<3; i++)
			printf("%6d", cost[i]);
		printf("%3c%8.2lf\n", c, sum);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
