/* @JUDGE_ID:	10319NX	222 C++*/
#include<stdio.h>
#include<stdlib.h>

typedef struct Station{
	double cost;
	double money;
	double dis;
}Station;

Station station[55];
int lim;
double capacity, mile, dis;

int comp(const void *p, const void *q)
{
	return (int)(((Station *)p)->dis - ((Station *)q)->dis);
}

void initial(double sum)
{
		lim++;
		dis = capacity*mile;
		station[0].dis = 0;
		station[lim].dis = sum;
		station[lim].cost = 100000.0;
		station[lim].money = 0.0;
}

int exist(double min, double max)
{
	int i;

	for(i=0; i<=lim; i++)
		if((station[i].dis-min)>0.000001 && (max-station[i].dis)>0.000001)
			return 1;
	return 0;
}

int main()
{
	double sum;
	double min, max;
	double cost;
	int i, j;
	int round=1;

	while(scanf("%lf", &sum) != EOF){
		if((sum+1.0) > -0.0000001 && (sum+1.0) < 0.0000001)
			break;
		scanf("%lf%lf%lf", &capacity, &mile, &station[0].cost);
		scanf("%d", &lim);
		initial(sum);
		for(i=1; i<lim; i++){
			scanf("%lf%lf", &station[i].dis, &station[i].money);
			station[i].money /= 100.0;
			station[i].cost = 100000.0;
		}
		qsort(station, lim, sizeof(station[0]), comp);
		for(i=0; i<lim; i++){
			if(!((station[i].cost-100000.0)>-0.0000001 && (station[i].cost-100000.0)<0.0000001)){
				if(exist(dis/2.0+station[i].dis, dis+station[i].dis)){
					min = dis/2.0+station[i].dis;
					max = dis+station[i].dis;
				}
				else{
					min = 0;
					max = dis/2.0+station[i].dis;
				}
				for(j=i+1; j<=lim; j++){
					if((station[j].dis-min)>-0.000001 && (max-station[j].dis)>-0.000001){
						cost = station[i].cost + (station[j].dis-station[i].dis)/mile*station[j].money + 2.0;
						if(cost < station[j].cost)
							station[j].cost = cost;
					}
				}
			}
		}
		printf("Data Set #%d\n", round);
		printf("minimum cost = $%.02lf\n", station[lim].cost-2.0);
		round++;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */